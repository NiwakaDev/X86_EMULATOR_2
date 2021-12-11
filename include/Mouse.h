#pragma once
#include "common.h"
#include "Fifo.h"
#include "IoDevice.h"

class Pic;

class Mouse:public IoDevice{
    private:
        bool enable_flg;
    public:
        Mouse();
        void Out8(uint16_t addr, uint8_t data);
        uint8_t In8(uint16_t addr);
        int IsEmpty();
        bool IsEnable(){
            return this->enable_flg;
        }

        void SetEnable(){
            this->enable_flg = true;
        }
};