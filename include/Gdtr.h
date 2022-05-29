#pragma once
#include "DescriptorTableRegister.h"
#include "common.h"

class DescriptorTableRegister;

class Gdtr : public DescriptorTableRegister {
 public:
  Gdtr(std::string name, uint16_t limit, uint32_t base);
};