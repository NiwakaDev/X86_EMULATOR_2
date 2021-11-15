#pragma once
#include "common.h"
#include "DescriptorTableRegister.h"

class DescriptorTableRegister;

class Gdtr:public DescriptorTableRegister{
    public:
        Gdtr(string name, uint16_t limit, uint32_t base);
};