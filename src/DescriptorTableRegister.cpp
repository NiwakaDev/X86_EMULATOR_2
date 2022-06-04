#include "DescriptorTableRegister.h"
using namespace std;

DescriptorTableRegister::DescriptorTableRegister(uint16_t limit,
                                                 uint32_t base) {
  this->limit = limit;
  this->base = base;
  this->obj = make_unique<Object>();
}

void DescriptorTableRegister::Set(uint16_t limit, uint32_t base) {
  this->limit = limit;
  this->base = base;
}

uint32_t DescriptorTableRegister::GetBase() { return this->base; }

uint16_t DescriptorTableRegister::GetLimit() { return this->limit; }