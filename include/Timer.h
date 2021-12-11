#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Pic;

class Timer:public IoDevice{
    private:
        bool enable = false;
        std::thread* timer_thread;
        uint32_t cycle;
        uint32_t clock;
        uint32_t mode;
        void Run();
    public:
        Timer();
        void ConsumeData();
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        int IsEmpty();
};