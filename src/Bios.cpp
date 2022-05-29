#include "Bios.h"
#include <fstream>
#include "BiosFunction.h"
#include "Cpu.h"
const int IPL_SIZE = 512;
using namespace std;

Bios::Bios(function<void(uint8_t* buff, int size)> read_callback, Vga& vga,
           Kbc& kbc) {
  for (int i = 0; i < Bios::BIOS_FUNCTION_SIZE; i++) {
    this->bios_functions[i] = NULL;
  }
  this->bios_functions[BIOS_FUNCTION::VIDEO_FUNCTION] =
      make_unique<VideoFunction>(vga);
  this->bios_functions[BIOS_FUNCTION::EQUIPMENTLIST_FUNCTION] =
      make_unique<EquipmentListFunction>();
  this->bios_functions[BIOS_FUNCTION::MEMORY_FUNCTION] =
      make_unique<MemoryFunction>();
  this->bios_functions[BIOS_FUNCTION::FLOPPY_FUNCTION] =
      make_unique<FloppyFunction>(read_callback);
  this->bios_functions[BIOS_FUNCTION::GENERAL_PURPOSE_FUNCTION] =
      make_unique<GeneralSystemServicesFunction>();
  this->bios_functions[BIOS_FUNCTION::KEY_FUNCTION] =
      make_unique<KeyFunction>(kbc);
  this->bios_functions[BIOS_FUNCTION::TIMER_FUNCTION] =
      make_unique<TimerFunction>();
  this->obj = make_unique<Object>();
}

Bios::~Bios() {}

void Bios::CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number) {
  if (this->bios_functions[bios_number].get() == NULL)
    this->obj->Error("Not implemented: 0x%02X at Bios::CallFunction",
                     bios_number);
  this->bios_functions[bios_number]->Run(cpu, mem);
}

// TODO :
// LoadIpl内の処理はBIOS::BIOSに移して、LoadIpl関数は廃止にする
void Bios::LoadIpl(
    function<void(uint8_t* buff, int size)> read_callback,
    function<void(uint32_t addr, uint8_t data)> memory_write8_callback) {
  uint8_t buff[IPL_SIZE];
  read_callback(buff, IPL_SIZE);
  for (int i = 0; i < IPL_SIZE; i++) {
    memory_write8_callback(IPL_START_ADDR + i, buff[i]);
  }
}