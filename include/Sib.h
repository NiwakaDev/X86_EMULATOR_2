#pragma once
#include "common.h"
class Cpu;

class Sib{
    public:
        void ParseSib(const uint8_t sib_byte, const uint8_t mod);
        uint32_t GetAddress(Cpu& cpu);
        uint32_t GetBase();
        uint32_t GetScale();
        uint32_t GetIndex();
        void SetDisp32(const uint32_t disp32);
    private:
        uint8_t scale;
        uint8_t index;
        uint8_t base;
        uint8_t mod;
        uint32_t disp32;
};