#pragma once
#include "common.h"
#include "DescriptorTableRegister.h"

class DescriptorTableRegister;

class Idtr:public DescriptorTableRegister{
    public:
        Idtr(std::string name, uint16_t limit, uint32_t base);
};