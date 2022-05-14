#pragma once
#include "common.h"

class Memory;
class Cpu;
class BiosFunction;
class Vga;
class Kbc;

class Bios{
    public:
        Bios(std::fstream& disk_image_straem, Vga& vga, Kbc& kbc);
        ~Bios();
        void LoadIpl(std::fstream& input_file, Memory& mem);
        void CallFunction(Cpu& cpu, Memory& mem, const uint8_t bios_number);
    private:
        enum{
            BIOS_FUNCTION_SIZE = 256,
        };
        enum BIOS_FUNCTION{
            VIDEO_FUNCTION           = 0x10,
            EQUIPMENTLIST_FUNCTION   = 0x11,
            MEMORY_FUNCTION          = 0x12,
            FLOPPY_FUNCTION          = 0x13,
            GENERAL_PURPOSE_FUNCTION = 0x15,
            KEY_FUNCTION             = 0x16,
            TIMER_FUNCTION           = 0x1A,
        };
        std::unique_ptr<BiosFunction> bios_functions[BIOS_FUNCTION_SIZE];
        std::unique_ptr<Object> obj;
};