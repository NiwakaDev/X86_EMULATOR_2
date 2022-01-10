#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Pic;

class Mouse:public IoDevice{
    public:
        Mouse();
        void Out8(const uint16_t addr, const uint8_t data);
        uint8_t In8(const uint16_t addr);
        int IsEmpty();
        bool IsEnable();
        void SetEnable();
    private:
        bool enable_flg;
};

#include "detail/Mouse.h"