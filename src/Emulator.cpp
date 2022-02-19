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

const int BIOS_ROM_SIZE = 65536;

Emulator::Emulator(int argc, char* argv[]){
    #ifdef DEBUG
        if(this->ParseArgv(argc, argv)<=1){
            fprintf(stderr, "Usage: ./x86 [ OPTIONS ]\n");
            fprintf(stderr, "       -image, -i : disk-image-name\n");
            fprintf(stderr, "               -d : debug\n");
            fprintf(stderr, "               -b : bios-name\n");
            exit(EXIT_FAILURE);
        }
    #else
        if(this->ParseArgv(argc, argv)==0){
            fprintf(stderr, "Usage: ./x86 [ OPTIONS ]\n");
            fprintf(stderr, "       -image, -i : disk-image-name\n");
            fprintf(stderr, "               -d : debug\n");
            fprintf(stderr, "               -b : bios-name\n");
            exit(EXIT_FAILURE);
        }
    #endif
    for(int i=0; i<16; i++){
        this->io_devices[i] = NULL;
    }
    this->mem     = make_unique<Memory>();
    this->fdc     = make_unique<Fdc>(this->disk_image_name);
    this->timer   = make_unique<Timer>();
    this->mouse   = make_unique<Mouse>();
    this->kbc     = make_unique<Kbc>(*(this->mouse.get()));
    this->io_devices[0x00] = this->timer.get();
    this->io_devices[0x01] = this->kbc.get();
    this->io_devices[0x06] = this->fdc.get();
    this->io_devices[0x0C] = this->mouse.get();
    this->pic     = make_unique<Pic>(this->io_devices);
    this->vga     = make_unique<Vga>(*(this->mem.get()));
    this->bios    = make_unique<Bios>(this->disk_image_name, *(this->vga.get()), *(this->kbc.get()));
    this->cpu     = make_unique<Cpu>(*(this->bios.get()), *(this->mem.get()));
    this->io_port = make_unique<IoPort>(*(this->vga.get()), *(this->pic.get()), *(this->kbc.get()), *(this->timer.get()), *(this->fdc.get()));
    this->gui     = make_unique<Gui>(*(this->vga.get()));
    this->gui->AddIoDevice(Gui::KBD, *(this->kbc.get()));
    this->gui->AddIoDevice(Gui::MOUSE, *(this->mouse.get()));
    this->fdc->gui = this->gui.get();
    this->bios->LoadIpl(this->disk_image_name, *(this->mem.get()));
    for(int i=0; i<0x20; i++){//8086runを参考にした
        this->mem->Write(i<<2, i);
    }
    #ifdef DEBUG
        //biosをロードする。
        fstream bios_stream;
        bios_stream.open(this->bios_name, ios::in|ios::binary);
        if(!bios_stream.is_open()){
            this->Error("Cant open : %s", this->bios_name);
        }
        bios_stream.read((char*)this->mem->GetPointer(0x000f0000), BIOS_ROM_SIZE);
        //bios_stream.read((char*)this->mem->GetPointer(0xffff0000), BIOS_ROM_SIZE);
        bios_stream.close();
    #endif
}

Emulator::~Emulator(){

}

int Emulator::ParseArgv(int argc, char* argv[]){
    #ifdef DEBUG
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
            parse_result += 1;
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
    #else
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
        return 0;
    }
   return parse_result;
    #endif
}

void Emulator::ThreadJoin(){
    this->emu_thread->join();
}

void Emulator::RunMainLoop(){
    #ifdef DEBUG
        this->MainLoop();
    #else
        this->emu_thread = make_unique<thread>(&Emulator::MainLoop, this);
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
        /***
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
        ***/
        if((this->cpu->IsException())||(this->cpu->IsFlag(IF)&&this->cpu->IsProtectedMode())){
            if(this->cpu->IsException()){
                this->cpu->HandleInterrupt(this->cpu->GetVectorNumber());
            }else if(this->pic->HasIrq()){
                this->cpu->HandleInterrupt(this->pic->GetNowIrq()+0x20);                
            }
        }
        if(this->cpu->Run(*this)){
            //正常
            continue;
        }else{
            this->gui->Finish();
            break;
        }
    }
}