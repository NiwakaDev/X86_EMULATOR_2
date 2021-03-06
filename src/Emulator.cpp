#include "Emulator.h"
#include <fstream>
#include "Bios.h"
#include "Cpu.h"
#include "Fdc.h"
#include "Gui.h"
#include "IoPort.h"
#include "Kbc.h"
#include "Memory.h"
#include "Mouse.h"
#include "Pic.h"
#include "Timer.h"
#include "Vga.h"
using namespace std;

#ifdef DEBUG
extern bool niwaka_start_flg;
#endif

Emulator::Emulator(int argc, char* argv[]) {
  this->obj = make_unique<Object>();
#ifdef DEBUG
  this->disk_image_name = "haribote.img";
  this->bios_name = "test386.bin";
#else
  if (this->ParseArgv(argc, argv) == 0) {
    fprintf(stderr, "Usage: ./x86 [ OPTIONS ]\n");
    fprintf(stderr, "       -image, -i : disk-image-name\n");
    fprintf(stderr, "               -d : debug\n");
    fprintf(stderr, "               -b : bios-name\n");
    exit(EXIT_FAILURE);
  }
#endif
  for (int i = 0; i < 16; i++) {
    this->io_devices[i] = NULL;
  }
  fstream disk_image_stream;
  disk_image_stream.open(disk_image_name, ios::in | ios::binary);
  if (!disk_image_stream.is_open()) {
    cerr << "can`t open " << this->disk_image_name << " at Emulator::Emulator"
         << endl;
    exit(EXIT_FAILURE);
  }
  auto file_read_callback = [&](uint8_t* buff, int size) {
    disk_image_stream.seekg(0);
    disk_image_stream.read((char*)buff, size);
  };
  try {
    this->mem = make_unique<Memory>(MEM_SIZE);
    this->fdc = make_unique<Fdc>(file_read_callback);
    this->timer = make_unique<Timer>();
    this->mouse = make_unique<Mouse>();
    this->kbc = make_unique<Kbc>(*(this->mouse.get()));
    // TODO : マジックナンバーを修正
    this->io_devices[0x00] = this->timer.get();
    this->io_devices[0x01] = this->kbc.get();
    this->io_devices[0x06] = this->fdc.get();
    this->io_devices[0x0C] = this->mouse.get();
    this->pic = make_unique<Pic>(this->io_devices);
    // TODO : 自動フォーマッタを使う。
    this->vga = make_unique<Vga>(
        [&](const uint32_t addr) { return this->mem->Read8(addr); });
    this->bios = make_unique<Bios>(file_read_callback, *(this->vga.get()),
                                   *(this->kbc.get()));
    this->cpu = make_unique<Cpu>(
        [&](Cpu& cpu, Memory& mem, const uint8_t bios_number) {
          this->bios->CallFunction(cpu, mem, bios_number);
        },
        *(this->mem.get()),
        [&](uint16_t addr) { return this->io_port->In8(addr); },
        [&](uint16_t addr, uint8_t data) { this->io_port->Out8(addr, data); });
    this->io_port = make_unique<IoPort>(
        *(this->vga.get()), *(this->pic.get()), *(this->kbc.get()),
        *(this->timer.get()), *(this->fdc.get()));
    this->gui = make_unique<Gui>(DEFAULT_HEIGHT, DEFAULT_WIDTH);
    this->gui->AddIoDevice(Gui::KBD,
                           [&](uint8_t data) { this->kbc->Push(data); });
    this->gui->AddIoDevice(Gui::MOUSE,
                           [&](uint8_t data) { this->mouse->Push(data); });
    this->bios->LoadIpl(file_read_callback, [&](uint32_t addr, uint8_t data) {
      this->mem->Write(addr, data);
    });
    for (int i = 0; i < 0x20; i++) {  // full.img and fd.imgで利用,
                                      // 8086runを参考
      this->mem->Write(i << 2, i);
    }
  } catch (const char* error_message) {
    cout << error_message << endl;
    exit(EXIT_FAILURE);
  } catch (const runtime_error& e) {
    cout << e.what() << endl;
    exit(EXIT_FAILURE);
  }
#ifdef DEBUG
  // biosをロードする。
  fstream bios_stream;
  bios_stream.open(this->bios_name, ios::in | ios::binary);
  if (!bios_stream.is_open()) {
    this->obj->Error("Cant open : %s", this->bios_name);
  }
  const int BIOS_ROM_SIZE = 65536;
  bios_stream.read((char*)this->mem->GetPointer(0x000f0000), BIOS_ROM_SIZE);
  // bios_stream.read((char*)this->mem->GetPointer(0xffff0000), BIOS_ROM_SIZE);
  bios_stream.close();
#endif
}

Emulator::~Emulator() {}

int Emulator::ParseArgv(int argc, char* argv[]) {
#ifdef DEBUG
  int parse_result = 0;
  argv++;
  argc--;
  while (argc > 0) {
    if ((strcmp("-image", argv[0]) == 0) || (strcmp("-i", argv[0]) == 0)) {
      this->disk_image_name = argv[1];
      argc -= 2;
      argv += 2;
      parse_result += 1;
      continue;
    }
    if ((strcmp("-bios", argv[0]) == 0) || (strcmp("-b", argv[0]) == 0)) {
      this->bios_name = argv[1];
      argc -= 2;
      argv += 2;
      parse_result += 1;
      continue;
    }
    return 0;
  }
  return parse_result;
#else
  int parse_result = 0;
  argv++;
  argc--;
  while (argc > 0) {
    if ((strcmp("-image", argv[0]) == 0) || (strcmp("-i", argv[0]) == 0)) {
      this->disk_image_name = argv[1];
      argc -= 2;
      argv += 2;
      parse_result += 1;
      continue;
    }
    return 0;
  }
  return parse_result;
#endif
}

void Emulator::ThreadJoin() { this->emu_thread->join(); }

void Emulator::RunMainLoop() {
#ifdef DEBUG
  this->MainLoop();
#else
  this->emu_thread = make_unique<thread>(&Emulator::MainLoop, this);
#endif
}

void Emulator::RunGuiThread() {
  this->gui->Display([&](Pixel* image, int* display_width, int* display_height,
                         std::function<void()> resize_callback) {
    this->vga->SetImage(image, display_width, display_height, resize_callback);
  });
}

void Emulator::MainLoop() {
#ifdef DEBUG
  fstream log_stream;
  log_stream.open("output.txt", ios::out);
  if (!log_stream.is_open()) {
    cerr << "ファイルが開けませんでした" << endl;
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "DEBUG\n");
#endif

  try {
    while (!this->gui->IsQuit()) {
      if ((this->cpu->IsException()) ||
          (this->cpu->IsFlag(IF) && this->cpu->IsProtectedMode())) {
        if (this->cpu->IsException()) {
          this->cpu->HandleInterrupt(this->cpu->GetVectorNumber());
        } else if (this->pic->HasIrq()) {
          this->cpu->HandleInterrupt(this->pic->GetNowIrq() + 0x20);
        }
      }
      this->cpu->Run();
    }
  } catch (const runtime_error& e) {
    this->cpu->ShowRegisters();
    cout << e.what() << endl;
    this->gui->Finish();
  } catch (const out_of_range& e) {
    this->cpu->ShowRegisters();
    cout << e.what() << endl;
    this->gui->Finish();
  } catch (const system_error& e) {
    this->cpu->ShowRegisters();
    cout << e.what() << endl;
    this->gui->Finish();
  } catch (const char* error_message) {
    this->cpu->ShowRegisters();
    cerr << error_message << endl;
    this->gui->Finish();
  }
}
