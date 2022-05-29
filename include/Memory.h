#pragma once
#include "common.h"
const uint32_t MEM_SIZE = 33554432;  // 2<<24

class Memory {
 private:
  std::unique_ptr<Object> obj;
  std::unique_ptr<uint8_t[]> buff;
  uint32_t mem_size_ = MEM_SIZE;

 public:
  explicit Memory(uint32_t mem_size);
  ~Memory();
  template <typename type>
  void Write(const uint32_t addr, const type data);
  uint8_t Read8(const uint32_t addr) const;
  uint16_t Read16(const uint32_t addr) const;
  uint32_t Read32(const uint32_t addr) const;
  uint8_t* GetPointer(const uint32_t addr) const;
  uint32_t GetMemSize();
};

#include "detail/Memory.h"