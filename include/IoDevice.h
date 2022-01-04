#pragma once
#include "common.h"
#include "Fifo.h"

class IoDevice:public Object{
    protected:
        Fifo<uint8_t>* fifo = NULL;
    public:
        IoDevice();
        virtual void Out8(const uint16_t addr, const uint8_t data) = 0;
        virtual uint8_t In8(const uint16_t addr)             = 0;
        virtual int IsEmpty()                          = 0;
        void Push(uint8_t data);
        uint8_t Pop();//リアルモードで使用する
        uint8_t Front();//読み込むだけ。リアルモードで使用する
};