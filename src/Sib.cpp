#include "Sib.h"
#include "Cpu.h"
using namespace std;

uint32_t Sib::GetAddress(Cpu& cpu) {
  uint32_t addr = 0;
  if (this->base == 4) {
    if (!cpu.IsSegmentOverride()) cpu.SetDataSelector(SS);
  }
  if (this->base == 5 && (this->mod == 1 || this->mod == 2)) {
    addr = cpu.GetR32(EBP);
    if (!cpu.IsSegmentOverride()) cpu.SetDataSelector(SS);
  } else if (this->base == 5 && this->mod == 0x00) {
    addr = this->disp32;
  }
  if (this->scale == 0) {
    if (this->base != 5) {
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->index);
    }
  }
  if (this->scale == 1) {
    if (this->base != 5) {
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->index) * 2;
    }
  }
  if (this->scale == 2) {
    if (this->base != 5) {
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->index) * 4;
    }
  }
  if (this->scale == 3) {
    if (this->base != 5) {
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->index) * 8;
    }
  }
  return addr;
}