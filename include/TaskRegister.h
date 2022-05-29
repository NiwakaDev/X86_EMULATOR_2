#pragma once
#include "SegmentRegister.h"
#include "common.h"

class Cpu;

class TaskRegister : public SegmentRegister {
 private:
  void SetCache(Cpu& cpu, uint16_t selector);

 public:
  TaskRegister(uint16_t data);
};