#pragma once
#include "common.h"
#include "Fifo.h"
class Pic;

class Timer:public Object{
    private:
        bool enable = false;
        Pic* pic;
        std::thread* timer_thread;
        uint32_t cycle;
        uint32_t clock;
        uint32_t mode;
        void Run();
    public:
        Timer(Pic* pic);
        bool IsEmpty();
        void ConsumeData();
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
};