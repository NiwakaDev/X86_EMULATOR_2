#pragma once
#include "common.h"
#include "SegmentRegister.h"

class Cpu;

class Ldtr:public SegmentRegister{
    private:
        void SetCache(Cpu* cpu, uint16_t selector);
    public:
        Ldtr(uint16_t data);
};