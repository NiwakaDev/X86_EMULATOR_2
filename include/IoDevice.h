#pragma once
#include "common.h"
#include "Fifo.h"

class IoDevice:public Object{
    protected:
        Fifo<uint8_t>* fifo = NULL;
        void ProcessCommand(uint8_t command);
    public:
        IoDevice();
        virtual void Out8(uint16_t addr, uint8_t data) = 0;
        virtual uint8_t In8(uint16_t addr)             = 0;
        virtual int IsEmpty()                          = 0;
        void Push(uint8_t data);
        uint8_t Pop();//リアルモードで使用する
        uint8_t Front();//読み込むだけ。リアルモードで使用する
};