#include "Sib.h"
using namespace std;

uint32_t Sib::GetAddress(
    function<uint32_t(GENERAL_PURPOSE_REGISTER32 general_purpose_register32)>
        get_r32,
    bool is_segment_override, function<void()> set_ss_data_selector) {
  uint32_t addr = 0;
  if (this->base == 4) {
    if (!is_segment_override) set_ss_data_selector();
  }
  if (this->base == 5 && (this->mod == 1 || this->mod == 2)) {
    addr = get_r32(EBP);
    if (!is_segment_override) set_ss_data_selector();
  } else if (this->base == 5 && this->mod == 0x00) {
    addr = this->disp32;
  }
  if (this->scale == 0) {
    if (this->base != 5) {
      addr = get_r32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + get_r32((GENERAL_PURPOSE_REGISTER32)this->index);
    }
  }
  if (this->scale == 1) {
    if (this->base != 5) {
      addr = get_r32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + get_r32((GENERAL_PURPOSE_REGISTER32)this->index) * 2;
    }
  }
  if (this->scale == 2) {
    if (this->base != 5) {
      addr = get_r32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + get_r32((GENERAL_PURPOSE_REGISTER32)this->index) * 4;
    }
  }
  if (this->scale == 3) {
    if (this->base != 5) {
      addr = get_r32((GENERAL_PURPOSE_REGISTER32)this->base);
    }
    if (this->index != 4) {
      addr = addr + get_r32((GENERAL_PURPOSE_REGISTER32)this->index) * 8;
    }
  }
  return addr;
}