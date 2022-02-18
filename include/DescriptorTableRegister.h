#pragma once
#include "common.h"

class DescriptorTableRegister:public Object{
    protected:
        uint16_t limit;
        uint32_t   base;
    public:
        DescriptorTableRegister(std::string name, uint16_t limit, uint32_t base);
        void Set(uint16_t limit, uint32_t base);
        uint32_t GetBase();
        uint16_t GetLimit();
};