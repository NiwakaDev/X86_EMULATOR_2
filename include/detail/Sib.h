#include "../Sib.h"

inline uint32_t Sib::GetBase() { return this->base; }

inline uint32_t Sib::GetScale() { return this->scale; }

inline uint32_t Sib::GetIndex() { return this->index; }

inline void Sib::SetDisp32(const uint32_t disp32) { this->disp32 = disp32; }

inline void Sib::ParseSib(const uint8_t sib_byte, const uint8_t mod) {
  this->scale = ((sib_byte & 0xc0) >> 6);
  this->index = ((sib_byte & 0x38) >> 3);
  this->base = sib_byte & 0x07;
  this->mod = mod;
}