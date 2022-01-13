#pragma once
#include "common.h"
const int BIOS_FUNCTION_SIZE = 256;

class Memory;
class Cpu;
class BiosFunction;
class Vga;
class Kbc;

class Bios:public Object{
    private:
        BiosFunction *bios_functions[BIOS_FUNCTION_SIZE];
    public:
        Bios(const char* const file_name, Vga& vga, Kbc& kbc);
        ~Bios();
        void LoadIpl(const char* const file_name, Memory& mem);
        void CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number);
};