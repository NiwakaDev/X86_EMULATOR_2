#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Fdc:public IoDevice{
    private:
        void ProcessCommand(uint8_t command);
    public:
        Fdc();
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        int IsEmpty();
};