#pragma once
#include "common.h"
#include "Fifo.h"

class Pic;

class Mouse:public Object{
    private:
        Pic* pic;
        bool enable_flg;
        Fifo<uint8_t>* fifo = NULL;
    public:
        Mouse(Pic* pic);
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        bool IsEnable();
        void SetEnable();
        void Send(uint8_t data);
        int IsEmpty();
};