#include "Bios.h"
#include "Memory.h"
#include "Cpu.h"
#include "BiosFunction.h"
#include <fstream>
const int IPL_SIZE = 512;
using namespace std;

Bios::Bios(const char* const file_name, Vga& vga, Kbc& kbc){
    for(int i=0; i<Bios::BIOS_FUNCTION_SIZE; i++){
        this->bios_functions[i] = NULL;
    }
    this->bios_functions[0x10] = make_unique<VideoFunction>(vga);
    this->bios_functions[0x11] = make_unique<EquipmentListFunction>();
    this->bios_functions[0x12] = make_unique<MemoryFunction>();
    this->bios_functions[0x13] = make_unique<FloppyFunction>(file_name);
    this->bios_functions[0x15] = make_unique<GeneralSystemServicesFunction>();
    this->bios_functions[0x16] = make_unique<KeyFunction>(kbc);
    this->bios_functions[0x1A] = make_unique<TimerFunction>();
}

Bios::~Bios(){

}

void Bios::CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number){
    if(this->bios_functions[bios_number].get()==NULL)this->obj->Error("Not implemented: 0x%02X at Bios::CallFunction", bios_number);
    this->bios_functions[bios_number]->Run(cpu, mem);
}

void Bios::LoadIpl(const char* const file_name, Memory& mem){
    fstream input_file;
    input_file.open(file_name, ios::in|ios::binary);
    if(!input_file.is_open()){
        this->obj->Error("Error: input_file.open at Bios::LoadIpl");
    }
    uint8_t buff[IPL_SIZE];
    input_file.read((char*)buff, IPL_SIZE);
    for(int i=0; i<IPL_SIZE; i++){
        mem.Write(IPL_START_ADDR+i, buff[i]);
    }
}