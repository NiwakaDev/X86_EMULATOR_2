#include "Bios.h"
#include "Memory.h"
#include "Cpu.h"
#include "BiosFunction.h"
#include <fstream>
const int IPL_SIZE = 512;
using namespace std;

Bios::~Bios(){
    for(int i=0; i<BIOS_FUNCTION_SIZE; i++){
        if(this->bios_functions[i]!=NULL){
            delete this->bios_functions[i];
        }
    }
}

Bios::Bios(const char* const file_name, Vga& vga, Kbc& kbc){
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

void Bios::CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number){
    if(this->bios_functions[bios_number]==NULL)this->Error("Not implemented: 0x%02X at Bios::CallFunction", bios_number);
    this->bios_functions[bios_number]->Run(cpu, mem);
}

void Bios::LoadIpl(const char* const file_name, Memory& mem){
    fstream input_file;
    input_file.open(file_name, ios::in|ios::binary);
    if(!input_file.is_open()){
        this->Error("Error: input_file.open at Bios::LoadIpl");
    }
    uint8_t* buff = new uint8_t[IPL_SIZE];
    input_file.read((char*)buff, IPL_SIZE);
    for(int i=0; i<IPL_SIZE; i++){
        mem.Write(IPL_START_ADDR+i, buff[i]);
    }
    input_file.close();
    delete[] buff;
}