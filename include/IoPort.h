#pragma once
#include "common.h"

class Vga;
class Pic;
class Kbc;
class Timer;
class Fdc;

class IoPort:public Object{
    private:
        Vga* vga = NULL;
        Pic* pic = NULL;
        Kbc* kbc = NULL;
        Timer* timer = NULL;
        Fdc* fdc = NULL;
    public:
        IoPort(Vga* vga, Pic* pic, Kbc* kbc, Timer* timer, Fdc* fdc);
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
};