#pragma once
#include "common.h"

class Memory;
class Cpu;
class BiosFunction;
class Vga;
class Kbc;

class Bios{
    public:
        Bios(const char* const file_name, Vga& vga, Kbc& kbc);
        ~Bios();
        void LoadIpl(const char* const file_name, Memory& mem);
        void CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number);
    private:
        enum{
            BIOS_FUNCTION_SIZE = 256,
        };
        std::unique_ptr<BiosFunction> bios_functions[BIOS_FUNCTION_SIZE];
        std::unique_ptr<Object> obj;
};