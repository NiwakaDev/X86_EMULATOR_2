#include "DescriptorTableRegister.h"
using namespace std;

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

uint16_t DescriptorTableRegister::GetLimit(){
    return this->limit;
}