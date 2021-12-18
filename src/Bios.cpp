#include "Bios.h"
#include "Memory.h"
#include "Cpu.h"
#include "BiosFunction.h"
#define IPL_SIZE 512
using namespace std;

Bios::~Bios(){
    for(int i=0; i<BIOS_FUNCTION_SIZE; i++){
        if(this->bios_functions[i]!=NULL){
            delete this->bios_functions[i];
        }
    }
}

Bios::Bios(char* file_name, Vga* vga, Kbc* kbc){
    for(int i=0; i<BIOS_FUNCTION_SIZE; i++){
        this->bios_functions[i] = NULL;
    }
    this->bios_functions[0x10] = new VideoFunction(vga);
    this->bios_functions[0x11] = new EquipmentListFunction();
    this->bios_functions[0x12] = new MemoryFunction();
    this->bios_functions[0x13] = new FloppyFunction(file_name);
    this->bios_functions[0x15] = new GeneralSystemServicesFunction();
    this->bios_functions[0x16] = new KeyFunction(kbc);
    this->bios_functions[0x1A] = new TimerFunction();
}

void Bios::CallFunction(Cpu *cpu, Memory* mem, uint8_t bios_number){
    if(this->bios_functions[bios_number]==NULL)this->Error("Not implemented: 0x%02X at Bios::CallFunction", bios_number);
    this->bios_functions[bios_number]->Run(cpu, mem);
}

void Bios::LoadIpl(char* file_name, Memory* mem){
    uint8_t* buff = NULL;
    FILE* fp      = NULL;
    int cnt;
    if((buff=(uint8_t*)malloc(IPL_SIZE))==NULL){
        this->Error("buff==NULL at Bios::LoadIpl");
    }
    if((fp=fopen(file_name, "rb"))==NULL){
        this->Error("can`t open %s at Bios::LoadIpl", file_name);
    }
    cnt=fread(buff, 1, IPL_SIZE, fp);
    for(int i=0; i<IPL_SIZE; i++){
        mem->Write(IPL_START_ADDR+i, buff[i]);
    }
    free(buff);
    fclose(fp);
}