#pragma once
#include "CpuEnum.h"
#include "common.h"

class Sib {
 public:
  inline void ParseSib(const uint8_t sib_byte, const uint8_t mod);
  uint32_t GetAddress(
      std::function<
          uint32_t(GENERAL_PURPOSE_REGISTER32 general_purpose_register32)>
          get_r32,
      bool is_segment_override, std::function<void()> set_ss_data_selector);
  inline uint32_t GetBase();
  inline uint32_t GetScale();
  inline uint32_t GetIndex();
  inline void SetDisp32(const uint32_t disp32);

 private:
  uint8_t scale;
  uint8_t index;
  uint8_t base;
  uint8_t mod;
  uint32_t disp32;
};

#include "detail/Sib.h"