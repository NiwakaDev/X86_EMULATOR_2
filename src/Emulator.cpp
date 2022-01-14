#include "Emulator.h"
#include "Bios.h"
#include "Memory.h"
#include "Cpu.h"
#include "IoPort.h"
#include "Vga.h"
#include "Gui.h"
#include "Pic.h"
#include "Kbc.h"
#include "Mouse.h"
#include "Timer.h"
#include "Fdc.h"
using namespace std;

#ifdef DEBUG
    extern bool niwaka_start_flg;
#endif 

const int BIOS_SIZE = 65536;

Emulator::Emulator(int argc, char* argv[]){
    if(this->ParseArgv(argc, argv)==0){
        fprintf(stderr, "Usage: ./x86 [ OPTIONS ]\n");
        fprintf(stderr, "       -image, -i : disk-image-name\n");
        fprintf(stderr, "               -d : debug\n");
        fprintf(stderr, "               -b : bios-name\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<16; i++){
        this->io_devices[i] = NULL;
    }
    this->mem     = new Memory();
    this->fdc     = new Fdc(this->disk_image_name);
    this->timer   = new Timer();
    this->mouse   = new Mouse();
    this->kbc     = new Kbc(*(this->mouse));
    this->io_devices[0x00] = this->timer;
    this->io_devices[0x01] = this->kbc;
    this->io_devices[0x06] = this->fdc;
    this->io_devices[0x0C] = this->mouse;
    this->pic     = new Pic(this->io_devices);
    this->vga     = new Vga(*(this->mem));
    this->bios    = new Bios(this->disk_image_name, *(this->vga), *(this->kbc));
    this->cpu     = new Cpu(*(this->bios), *(this->mem));
    this->io_port = new IoPort(*(this->vga), *(this->pic), *(this->kbc), *(this->timer), *(this->fdc));
    this->gui     = new Gui(*(this->vga));
    this->gui->AddIoDevice(Gui::KBD, *(this->kbc));
    this->gui->AddIoDevice(Gui::MOUSE, *(this->mouse));
    this->fdc->gui = this->gui;
    this->bios->LoadIpl(this->disk_image_name, *(this->mem));
    for(int i=0; i<0x20; i++){//8086runを参考にした
        this->mem->Write(i<<2, i);
    }
    /***
    //biosをロードする。
    fstream bios_stream;
    bios_stream.open(this->bios_name, ios::in|ios::binary);
    if(!bios_stream.is_open()){
        this->Error("Cant open : %s", this->bios_name);
    }
    bios_stream.read((char*)this->mem->GetPointer(0x000f0000), BIOS_SIZE);
    bios_stream.read((char*)this->mem->GetPointer(0xffff0000), BIOS_SIZE);
    bios_stream.close();
    ***/
}

Emulator::~Emulator(){
    delete this->gui;
    delete this->io_port;
    delete this->cpu;
    delete this->bios;
    delete this->vga;
    delete this->pic;
    delete this->kbc;
    delete this->mem;
}

int Emulator::ParseArgv(int argc, char* argv[]){
    int parse_result = 0;
    argv++;
    argc--;
    while(argc>0){
        if ((strcmp("-image", argv[0])==0) || (strcmp("-i", argv[0])==0)) {
            this->disk_image_name = argv[1];
            argc -= 2;
            argv += 2;
            parse_result += 1;
            continue;
        }
        if ((strcmp("-bios", argv[0])==0) || (strcmp("-b", argv[0])==0)) {
            this->bios_name = argv[1];
            argc -= 2;
            argv += 2;
            //parse_result += 1;
            continue;
        }
        //プログラムを動かしてみたい時のオプション
        //私のために用意しているオプションなので、指定しないことをお勧めします。
        if ((strcmp("-headstart", argv[0])==0) || (strcmp("-h", argv[0])==0)) {//バイナリファイルの先頭から実行する。
            this->head_start = true;
            argc -= 1;
            argv += 1;
            continue;
        }
        return 0;
    }
   return parse_result;
}

void Emulator::ThreadJoin(){
    this->emu_thread->join();
}

void Emulator::RunMainLoop(){
    #ifdef DEBUG
        this->Run();
    #else
        this->emu_thread = new thread(&Emulator::MainLoop, this);
        this->gui->Display();
    #endif
}

void Emulator::MainLoop(){
    #ifdef DEBUG
        FILE* out = NULL;
        out = fopen("niwaka_output.txt", "w");
        if(out==NULL){
            fprintf(stderr, "Error: fopen");
            exit(1);
        }
        fprintf(stderr, "DEBUG\n");
    #endif
    
    //TODO : テストコードのせいで汚くなったメイン関数の修正
    while(!this->gui->IsQuit()){
        #ifdef DEBUG
            static int i = 0;
            static bool log = false;
            bool result;
            if(niwaka_start_flg){
                log = true;
            }
            if(this->cpu->IsFlag(IF)&&this->cpu->IsProtectedMode()){
                int irq_num;
                if((irq_num=this->pic->HasIrq(this->kbc, this->timer))!=-1){
                    this->cpu->HandleInterrupt(irq_num);                
                }
            }
            if(log)fprintf(out, "i:%d\n", i);
            if(log)this->cpu->Debug(out, true);
            result = this->cpu->Run(this);
            if(log)this->cpu->Debug(out, true);
            if(log)i++;
            if(!result||i==1000000){
                fclose(out);
                return;
            }
        #else
            if(this->cpu->IsFlag(IF)&&this->cpu->IsProtectedMode()){
                if(this->pic->HasIrq()){
                    this->cpu->HandleInterrupt(this->pic->GetNowIrq());                
                }
            }
            if(this->cpu->Run(*this)){
                //正常
                continue;
            }else{
                this->gui->Finish();
                break;
            }
        #endif
    }
}