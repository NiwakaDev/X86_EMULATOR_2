#pragma once
#include "common.h"
class Cpu;
class Memory;
class Vga;

class BiosFunction:public Object{
    protected:
        std::string function_name;
    public:
        BiosFunction();
        virtual void Run(Cpu *cpu, Memory* mem);
};

class VideoFunction:public BiosFunction{
    private:
        uint8_t color_table[8] = {30, 34, 32, 36, 31, 35, 33, 37};
        void out8(uint16_t addr, uint8_t value);
        Vga* vga;
    public:
        VideoFunction(Vga* vga);
        void Run(Cpu *cpu, Memory* mem);
};

class FloppyFunction:public BiosFunction{
    private:
        int32_t processed_sector_number = 0;
        int32_t sector_number   = 0;
        int32_t cylinder_number = 0;
        int32_t head_number = 0;
        int32_t drive_number = 0;
        int32_t es=0;
        int32_t bx=0;
        int32_t floppy_size = 1474560;
        uint8_t* buff=NULL;
        FILE* disk_img_stream;
        void Read(Cpu *cpu, Memory* mem);
        void Init(char* file_name);
    public:
        void Run(Cpu *cpu, Memory* mem);
        FloppyFunction(char* file_name);
};  

class KeyFunction:public BiosFunction{
    public:
        KeyFunction();
        void Run(Cpu *cpu, Memory* mem);
};