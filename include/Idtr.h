#pragma once
#include "common.h"
#include "DescriptorTableRegister.h"

class DescriptorTableRegister;

class Idtr:public DescriptorTableRegister{
    public:
        Idtr(string name, uint16_t limit, uint32_t base);
};