#include "TaskRegister.h"
using namespace std;

TaskRegister::TaskRegister(uint16_t data) : SegmentRegister(data) {
  this->selector.raw = data;
  this->cache.base_addr = 0x0;
  this->cache.limit = 0x0000FFFF;
}