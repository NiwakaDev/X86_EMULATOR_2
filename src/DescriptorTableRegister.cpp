#include "DescriptorTableRegister.h"

DescriptorTableRegister::DescriptorTableRegister(string name, uint16_t limit, uint32_t base){
    this->limit = limit;
    this->base  = base;
}   

void DescriptorTableRegister::Set(uint16_t limit, uint32_t base){
    this->limit = limit;
    this->base  = base;
}

uint32_t DescriptorTableRegister::GetBase(){
    return this->base;
}