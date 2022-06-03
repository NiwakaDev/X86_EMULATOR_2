#include "../Memory.h"

inline template <typename type>
void Memory::Write(const uint32_t addr, const type data) {
  if ((mem_size_ - sizeof(data) + 1) <= addr) {
    return;
  }
  *(type*)(this->buff.get() + addr) = data;
}

inline uint8_t Memory::Read8(const uint32_t addr) const {
  if ((mem_size_) <= addr) {
    //TODO: エラーメッセージにアドレスを埋め込むようにする。
    throw std::out_of_range("Invalid addr");
  }
  return this->buff[addr];
}

inline uint16_t Memory::Read16(const uint32_t addr) const {
  if ((mem_size_ - 1) <= addr) {
    throw std::out_of_range("Invalid addr");
  }
  uint16_t* data = (uint16_t*)(this->buff.get() + addr);
  return *data;
}


inline uint32_t Memory::Read32(const uint32_t addr) const {
  if ((mem_size_ - 3) <= addr) {
    //MEMO : はりぼてOSはメモリーのサイズよりも大きい番地にアクセスすることで、
    //大きさを推定している。つまり、Read32では例外を投げることができない。
    //Read8とRead16の例外を廃止にする方がいいかもしれない。
    return 0;
    //throw std::out_of_range("Invalid addr");
  }
  uint32_t* data = (uint32_t*)(this->buff.get() + addr);
  return *data;
}

inline uint8_t* Memory::GetPointer(const uint32_t addr) const {
  if (this->mem_size_ <= addr) {
    return NULL;
  }
  return this->buff.get() + addr;
}

inline uint32_t Memory::GetMemSize() { return this->mem_size_; }