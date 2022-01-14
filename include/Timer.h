#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Pic;

class Timer:public IoDevice{
    public:
        Timer();
        ~Timer();
        void ConsumeData();
        void Out8(const uint16_t addr, const uint8_t data);
        uint8_t In8(const uint16_t addr);
        int IsEmpty();
    private:
        Timer(const Timer& other);
        void operator=(const Timer& other);
        bool enable = false;
        std::thread* timer_thread;
        uint32_t cycle;
        void Run();
};