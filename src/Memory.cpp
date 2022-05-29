#include "Memory.h"
using namespace std;

Memory::Memory(uint32_t mem_size) : mem_size_(mem_size) {
  this->obj = make_unique<Object>();
  this->buff = make_unique<uint8_t[]>(MEM_SIZE);
}

Memory::~Memory() {}