#include "Bios.h"
#include "Memory.h"
#include "Cpu.h"
#include "BiosFunction.h"
#define IPL_SIZE 512

Bios::Bios(char* file_name){
    for(int i=0; i<BIOS_FUNCTION_SIZE; i++){
        this->bios_functions[i] = NULL;
    }
    this->bios_functions[0x10] = new VideoFunction();
    this->bios_functions[0x13] = new FloppyFunction(file_name);
    this->bios_functions[0x16] = new KeyFunction();
}

void Bios::CallFunction(Cpu *cpu, Memory* mem, uint8_t bios_number){
    if(this->bios_functions[bios_number]==NULL)this->Error("Not implemented: 0x%02X at Bios::CallFunction", bios_number);
    this->bios_functions[bios_number]->Run(cpu, mem);
}

void Bios::LoadIpl(char* file_name, Memory* mem){
    uint8_t* buff = NULL;
    FILE* fp      = NULL;
    if((buff=(uint8_t*)malloc(IPL_SIZE))==NULL){
        this->Error("buff==NULL at Bios::LoadIpl");
    }
    if((fp=fopen(file_name, "rb"))==NULL){
        this->Error("can`t open %s at Bios::LoadIpl", file_name);
    }
    if(fread(buff, 1, IPL_SIZE, fp)<IPL_SIZE){
        this->Error("can`t open %s at Bios::LoadIpl", file_name);
    }
    for(int i=0; i<IPL_SIZE; i++){
        mem->Write(IPL_START_ADDR+i, buff[i]);
    }
    free(buff);
    fclose(fp);
}