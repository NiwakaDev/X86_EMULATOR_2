#pragma once
#include "common.h"

class DescriptorTableRegister {
 protected:
  std::unique_ptr<Object> obj;
  uint16_t limit;
  uint32_t base;

 public:
  DescriptorTableRegister(uint16_t limit, uint32_t base);
  void Set(uint16_t limit, uint32_t base);
  uint32_t GetBase();
  uint16_t GetLimit();
};