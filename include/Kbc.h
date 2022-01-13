#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Pic;
class Mouse;

class Kbc:public IoDevice{
    private:
        Mouse* mouse = NULL;
        void ProcessCommand(uint8_t command);
    public:
        Kbc(Mouse& mouse);
        void Out8(const uint16_t addr, const uint8_t data);
        uint8_t In8(const uint16_t addr);
        int IsEmpty();
};