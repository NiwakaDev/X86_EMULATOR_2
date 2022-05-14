#include "Bios.h"
#include "Memory.h"
#include "Cpu.h"
#include "BiosFunction.h"
#include <fstream>
const int IPL_SIZE = 512;
using namespace std;

Bios::Bios(fstream& disk_image_stream, Vga& vga, Kbc& kbc){
    for(int i=0; i<Bios::BIOS_FUNCTION_SIZE; i++){
        this->bios_functions[i] = NULL;
    }
    this->bios_functions[BIOS_FUNCTION::VIDEO_FUNCTION]           = make_unique<VideoFunction>(vga);
    this->bios_functions[BIOS_FUNCTION::EQUIPMENTLIST_FUNCTION]   = make_unique<EquipmentListFunction>();
    this->bios_functions[BIOS_FUNCTION::MEMORY_FUNCTION]          = make_unique<MemoryFunction>();
    this->bios_functions[BIOS_FUNCTION::FLOPPY_FUNCTION]          = make_unique<FloppyFunction>(disk_image_stream);
    this->bios_functions[BIOS_FUNCTION::GENERAL_PURPOSE_FUNCTION] = make_unique<GeneralSystemServicesFunction>();
    this->bios_functions[BIOS_FUNCTION::KEY_FUNCTION]             = make_unique<KeyFunction>(kbc);
    this->bios_functions[BIOS_FUNCTION::TIMER_FUNCTION]           = make_unique<TimerFunction>();
    this->obj = make_unique<Object>();
}

Bios::~Bios(){

}

void Bios::CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number){
    if(this->bios_functions[bios_number].get()==NULL)this->obj->Error("Not implemented: 0x%02X at Bios::CallFunction", bios_number);
    this->bios_functions[bios_number]->Run(cpu, mem);
}

//TODO : 
//LoadIpl内の処理はBIOS::BIOSに移して、LoadIpl関数は廃止にする
void Bios::LoadIpl(fstream& input_file, Memory& mem){
    uint8_t buff[IPL_SIZE];
    input_file.read((char*)buff, IPL_SIZE);
    for(int i=0; i<IPL_SIZE; i++){
        mem.Write(IPL_START_ADDR+i, buff[i]);
    }
}