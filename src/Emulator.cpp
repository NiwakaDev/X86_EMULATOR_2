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
using namespace std;

extern bool niwaka_start_flg;

Emulator::~Emulator(){
    cerr << "Emulatorのデストラクタ" << endl;
    delete this->bios;
    delete this->cpu;
}

Emulator::Emulator(int argc, char* argv[]){
    int parse_result;
    parse_result  = this->ParseArgv(argc, argv);
    if(parse_result==0){
        fprintf(stderr, "Usage: ./x86 [ OPTIONS ]\n");
        fprintf(stderr, "       -image, -i : disk-image-name\n");
        fprintf(stderr, "               -d : debug\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<16; i++){
        this->io_devices[i] = NULL;
    }
    this->mem     = new Memory();
    this->timer   = new Timer();
    this->mouse   = new Mouse();
    this->kbc     = new Kbc(this->mouse);
    this->io_devices[0x00] = this->timer;
    this->io_devices[0x01] = this->kbc;
    this->io_devices[0x0C] = this->mouse;
    this->pic     = new Pic(this->io_devices);
    this->vga     = new Vga(this->mem);
    this->bios    = new Bios(this->disk_image_name, this->vga, this->kbc);
    this->cpu     = new Cpu(this->bios, this->mem);
    this->io_port = new IoPort(this->vga, this->pic, this->kbc, this->timer);
    this->gui     = new Gui(this->vga, this->kbc, this->mouse);
    this->bios->LoadIpl(this->disk_image_name, this->mem);
    for(int i=0; i<0x20; i++){//8086runを参考にした
        this->mem->Write(i<<2, i);
    }
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
            parse_result = 1;
            continue;
        }
        if ((strcmp("-debug", argv[0])==0) || (strcmp("-d", argv[0])==0)) {
            this->debug = true;
            argc -= 1;
            argv += 1;
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

void Emulator::Join(){
    this->emu_thread->join();
}

void Emulator::Start(){
    if(!this->debug){
        this->emu_thread = new thread(&Emulator::Run, this);
        this->gui->Display();
    }else{
        this->Run();
    }
}

void Emulator::Run(){
    int irq_num;
    FILE* out = NULL;
    #ifdef DEBUG
        out = fopen("niwaka_output.txt", "w");
        if(out==NULL){
            fprintf(stderr, "Error: fopen");
            exit(1);
        }
        fprintf(stderr, "DEBUG\n");
    #endif
    
    int  i = 0;
    bool result;
    bool log = false;
    //TODO : テストコードのせいで汚くなったメイン関数の修正
    while(!this->gui->IsQuit()){
        #ifdef DEBUG
            if(niwaka_start_flg){
                log = true;
            }
            if(this->cpu->IsFlag(IF)&&this->cpu->IsProtectedMode()){
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
                if((irq_num=this->pic->HasIrq(this->kbc, this->timer))!=-1){
                    this->cpu->HandleInterrupt(irq_num);                
                }
            }
            if(!this->cpu->Run(this)){
                this->gui->Finish();
                break;
            }
        #endif
    }
}