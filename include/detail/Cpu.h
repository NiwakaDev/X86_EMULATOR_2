#include "../Cpu.h"

inline void Cpu::AddEip(uint32_t data) {
  this->eip += data;
  if (this->IsRealMode()) {
    this->eip = this->eip & 0x0000FFFF;
  }
}

inline void Cpu::SetEip(uint32_t addr) {
  if (this->IsRealMode()) {
    this->eip = this->eip & 0x0000FFFF;
  }
  this->eip = addr;
}

inline bool Cpu::IsProtectedMode() const { return (bool)this->cr0.flgs.PE; }

inline bool Cpu::IsRealMode() const { return !((bool)this->cr0.flgs.PE); }

inline bool Cpu::IsPrefixAddrSize() { return this->prefix_flgs[FLG_67]; }

inline bool Cpu::IsPrefixOpSize() { return this->prefix_flgs[FLG_66]; }

inline void Cpu::SetR8L(GENERAL_PURPOSE_REGISTER32 register_type,
                        uint8_t data) {
  *this->gprs[register_type] =
      ((*this->gprs[register_type]) & 0xffffff00) | (uint32_t)data;
}

inline void Cpu::SetR8H(GENERAL_PURPOSE_REGISTER32 register_type,
                        uint8_t data) {
  *this->gprs[register_type] =
      ((*this->gprs[register_type]) & 0xffff00FF) | ((uint32_t)data << 8);
}

inline void Cpu::SetR8(uint32_t register_type, uint8_t data) {
  if (register_type < 4) {
    this->SetR8L((GENERAL_PURPOSE_REGISTER32)register_type, data);
  } else {
    this->SetR8H((GENERAL_PURPOSE_REGISTER32)(register_type - 4), data);
  }
}

inline void Cpu::SetR16(GENERAL_PURPOSE_REGISTER32 register_type,
                        uint16_t data) {
  *this->gprs[register_type] =
      ((*this->gprs[register_type]) & 0xffff0000) | (uint32_t)data;
}

inline void Cpu::SetR32(GENERAL_PURPOSE_REGISTER32 register_type,
                        uint32_t data) {
  *this->gprs[register_type] = data;
}

inline void Cpu::SetDataSelector(SEGMENT_REGISTER register_type) {
  this->default_data_selector = register_type;
}

inline void Cpu::SetEflgs(uint32_t eflgs) {
  this->eflags.raw = eflgs | 0x00000002;
  this->eflags.flgs.ID = 0;  // not support cpuid
}

inline uint8_t Cpu::GetR8(uint32_t register_type) {
  if (register_type < 4) {
    return this->GetR8L((GENERAL_PURPOSE_REGISTER32)register_type);
  } else {
    return this->GetR8H((GENERAL_PURPOSE_REGISTER32)(register_type - 4));
  }
}

inline uint8_t Cpu::GetR8L(GENERAL_PURPOSE_REGISTER32 register_type) {
  return (uint8_t)((*this->gprs[register_type]) & 0x000000ff);
}

inline uint8_t Cpu::GetR8H(GENERAL_PURPOSE_REGISTER32 register_type) {
  return (uint8_t)(((*this->gprs[register_type]) >> 8) & 0xff);
}

inline uint16_t Cpu::GetR16(GENERAL_PURPOSE_REGISTER32 register_type) {
  return (*this->gprs[register_type]) & 0x0000FFFF;
}

inline uint32_t Cpu::GetR32(GENERAL_PURPOSE_REGISTER32 register_type) {
  return *this->gprs[register_type];
}

inline uint32_t Cpu::GetEip() { return this->eip; }

inline uint32_t Cpu::GetEflgs() { return this->eflags.raw; }

template <typename type>
type Cpu::Inc(type data) {
  this->UpdateEflagsForInc(data);
  return data + 1;
}

inline template <typename type>
void Cpu::UpdateEflagsForInc(type d) {
  type result = (type)(d + 1);
  this->UpdateZF(result);
  this->UpdateSF(result);
  this->UpdatePF(result);
  this->UpdateOF_Add(result, d, (type)1);
}

template <typename type>
type Cpu::Dec(type data) {
  type result = (type)(data - 1);
  this->UpdateEflagsForDec(result, data, (type)1);
  return result;
}

inline template <typename type>
void Cpu::UpdateEflagsForDec(type result, type d1, type d2) {
  this->UpdateZF(result);
  this->UpdateSF(result);
  this->UpdatePF(result);
  switch (sizeof(result)) {
    case 1:
      this->UpdateOF_Sub8(result, d1, d2);
      break;
    case 2:
      this->UpdateOF_Sub16(result, d1, d2);
      break;
    case 4:
      this->UpdateOF_Sub(result, d1, d2);
      break;
    default:
      this->obj->Error(
          "Not implemented: data_size=%dbyte at Cpu::UpdateElfagsForDec",
          sizeof(result));
  }
}

inline template <typename type>
type Cpu::Adc(type data1, type data2, type carry) {
  this->UpdateEflagsForAdc(data1, data2, carry);
  return data1 + data2 + carry;
}

inline template <typename type>
type Cpu::Xor(type data1, type data2) {
  type result = data1 ^ data2;
  this->UpdateEflagsForAnd(result);
  return result;
}

inline template <typename type>
type Cpu::Or(type data1, type data2) {
  this->UpdateEflagsForAnd((type)(data1 | data2));
  return data1 | data2;
}

inline template <typename type>
type Cpu::And(type data1, type data2) {
  this->UpdateEflagsForAnd((type)(data1 & data2));
  return data1 & data2;
}

inline template <typename type>
type Cpu::Add(type data1, type data2) {
  this->UpdateEflagsForAdd(data1, data2);
  return data1 + data2;
}

inline template <typename type>
type Cpu::Sub(type data1, type data2) {
  this->UpdateEflagsForSub(data1, data2);
  return data1 - data2;
}

inline template <typename type>
void Cpu::UpdateEflagsForSub(type data1, type data2) {
  this->UpdateZF((type)(data1 - data2));
  this->UpdatePF(data1 - data2);
  this->UpdateSF((type)(data1 - data2));
  if (sizeof(type) == 1) {
    this->UpdateCfForSub((uint16_t)((uint16_t)data1 - (uint16_t)data2),
                         sizeof(type));
  } else if (sizeof(type) == 2) {
    this->UpdateCfForSub((uint32_t)((uint32_t)data1 - (uint32_t)data2),
                         sizeof(type));
  } else if (sizeof(type) == 4) {
    this->UpdateCfForSub((uint64_t)((uint64_t)data1 - (uint64_t)data2),
                         sizeof(type));
  } else {
    this->obj->Error("Not implemented: sizeof(type)=%d\n", sizeof(type));
  }
  switch (sizeof(type)) {
    case 1:
      this->UpdateOF_Sub8(data1 - data2, data1, data2);
      break;
    case 2:
      this->UpdateOF_Sub16(data1 - data2, data1, data2);
      break;
    case 4:
      this->UpdateOF_Sub(data1 - data2, data1, data2);
      break;
    default:
      this->obj->Error(
          "Not implemented: data_size=%dbyte at Cpu::UpdateElfagsForDec",
          sizeof(type));
  }
}

template <typename type>
void Cpu::UpdateEflagsForAdc(type d1, type d2, type c) {
  this->UpdateZF((type)(d1 + d2 + c));
  this->UpdateSF((type)(d1 + d2 + c));
  this->UpdatePF((type)(d1 + d2 + c));
  if (sizeof(d1) == 1) {
    this->UpdateCfForSub((uint16_t)((uint16_t)d1 + (uint16_t)d2 + (uint16_t)c),
                         sizeof(d1));
  } else if (sizeof(d1) == 2) {
    this->UpdateCfForSub((uint32_t)((uint32_t)d1 + (uint32_t)d2 + (uint32_t)c),
                         sizeof(d1));
  } else if (sizeof(d1) == 4) {
    this->UpdateCfForSub((uint64_t)((uint64_t)d1 + (uint64_t)d2 + (uint64_t)c),
                         sizeof(d1));
  } else {
    this->obj->Error(
        "Not implemented: sizeof(type2)=%d at Cpu::UpdateEflagsForAdc\n",
        sizeof(d1));
  }
  type result = d1 + d2 + c;
  bool of_flg;
  if (sizeof(d1) == 1) {
    of_flg = ((~(d1 ^ d2)) & (d1 ^ result) & 0x80) != 0x00;
  } else if (sizeof(d1) == 2) {
    of_flg = ((~(d1 ^ d2)) & (d1 ^ result) & 0x8000) != 0x0000;
  } else if (sizeof(d1) == 4) {
    of_flg = ((~(d1 ^ d2)) & (d1 ^ result) & 0x80000000) != 0x00000000;
  } else {
    this->obj->Error(
        "Not implemented: sizeof(d1)=%d at Cpu::UpdateEflagsForAdc\n",
        sizeof(d1));
  }
  if (of_flg) {
    this->SetFlag(OF);
  } else {
    this->ClearFlag(OF);
  }
}

inline template <typename type>
void Cpu::UpdateEflagsForAdd(type d1, type d2) {
  this->UpdateZF((type)(d1 + d2));
  this->UpdateSF((type)(d1 + d2));
  this->UpdatePF((type)(d1 + d2));
  if (sizeof(d1) == 1) {
    this->UpdateCfForSub((uint16_t)((uint16_t)d1 + (uint16_t)d2), sizeof(d1));
  } else if (sizeof(d1) == 2) {
    this->UpdateCfForSub((uint32_t)((uint32_t)d1 + (uint32_t)d2), sizeof(d1));
  } else if (sizeof(d1) == 4) {
    this->UpdateCfForSub((uint64_t)((uint64_t)d1 + (uint64_t)d2), sizeof(d1));
  } else {
    this->obj->Error("Not implemented: sizeof(type)=%d\n", sizeof(type));
  }
  this->UpdateOF_Add((type)(d1 + d2), (type)d1, (type)d2);
}

template <typename type>
type Cpu::Sal(type data, type count) {
  type masked_count = count & 0x1F;
  if (masked_count == 0) {
    return data;
  }
  if (masked_count == 1) {
    bool next_msb;  //最上位bitをn番目とすると、これはn-1番目のbit
    bool cf;
    if (sizeof(data) == 1) {
      next_msb = (0x40 & data) != 0;
      cf = (0x80 & data) != 0;
    } else if (sizeof(data) == 2) {
      next_msb = (0x4000 & data) != 0;
      cf = (0x8000 & data) != 0;
    } else if (sizeof(data) == 4) {
      next_msb = (0x40000000 & data) != 0;
      cf = (0x80000000 & data) != 0;
    } else {
      this->obj->Error("Not implemented: sizeof(data)=%d", sizeof(data));
    }
    if (next_msb ^ cf) {
      this->SetFlag(OF);
    } else {
      this->ClearFlag(OF);
    }
  }
  for (int i = 0; i < masked_count; i++) {
    if (sizeof(data) == 1) {
      if ((0x80 & data) != 0) {
        this->SetFlag(CF);
      } else {
        this->ClearFlag(CF);
      }
    } else if (sizeof(data) == 2) {
      if ((0x8000 & data) != 0) {
        this->SetFlag(CF);
      } else {
        this->ClearFlag(CF);
      }
    } else if (sizeof(data) == 4) {
      if ((0x80000000 & data) != 0) {
        this->SetFlag(CF);
      } else {
        this->ClearFlag(CF);
      }
    }
    data = data << 1;
  }
  this->UpdateEflagsForShr(data);
  return data;
}

template <typename type>
type Cpu::Sar(type data, type count) {
  type masked_count = count & 0x1F;
  if (masked_count == 0) {
    return data;
  }
  if (masked_count == 1) {
    this->ClearFlag(OF);
  }
  type sign = 0;
  if (sizeof(data) == 1) {
    if ((data & 0x80) == 0x80) {
      sign = 0x80;
    }
  } else if (sizeof(data) == 2) {
    if ((data & 0x8000) == 0x8000) {
      sign = 0x8000;
    }
  } else if (sizeof(data) == 4) {
    if ((data & 0x80000000) == 0x80000000) {
      sign = 0x80000000;
    }
  } else {
    this->obj->Error("Not implemented: sizeof(data)=%d", sizeof(data));
  }
  for (int i = 0; i < masked_count; i++) {
    if (data & 0x01) {
      this->SetFlag(CF);
    } else {
      this->ClearFlag(CF);
    }
    data = data >> 1;
    data = data | sign;
  }
  this->UpdateEflagsForShr(data);
  return data;
}

template <typename type>
type Cpu::Shr(type data, type count) {
  type masked_count = count & 0x1F;
  if (masked_count == 0) {
    return data;
  }
  if (masked_count == 1) {
    type msb;
    if (sizeof(data) == 1) {
      msb = 0x80;
    } else if (sizeof(data) == 2) {
      msb = 0x8000;
    } else if (sizeof(data) == 4) {
      msb = 0x80000000;
    } else {
      this->obj->Error("Not implemented: sizeof(data)=%d", sizeof(data));
    }
    if ((data & msb) != 0) {
      this->SetFlag(OF);
    } else {
      this->ClearFlag(OF);
    }
  }
  for (int i = 0; i < masked_count; i++) {
    if (data & 0x01) {
      this->SetFlag(CF);
    } else {
      this->ClearFlag(CF);
    }
    data = data >> 1;
  }
  this->UpdateEflagsForShr(data);
  return data;
}

template <typename type>
type Cpu::Ror(type data, type count) {
  type masked_count;
  type msb;
  type next_msb;
  if (sizeof(data) == 1) {
    masked_count = count % 8;
    msb = 0x80;
    next_msb = 0x40;
  } else if (sizeof(data) == 2) {
    masked_count = count % 16;
    msb = 0x8000;
    next_msb = 0x4000;
  } else if (sizeof(data) == 4) {
    masked_count = count % 32;
    msb = 0x80000000;
    next_msb = 0x40000000;
  } else {
    this->obj->Error("Not implemented: sizeof(data)=%d", sizeof(data));
  }
  for (int i = 0; i < masked_count; i++) {
    if (data & 0x01) {
      this->SetFlag(CF);
    } else {
      this->ClearFlag(CF);
    }
    data = data >> 1;
    if (this->IsFlag(CF)) {
      data = data | msb;
    }
  }

  if (masked_count == 1) {
    bool msb_flg = (data & msb) != 0;
    bool next_msb_flg = (data & next_msb) != 0;
    if (msb_flg ^ next_msb_flg) {
      this->SetFlag(OF);
    } else {
      this->ClearFlag(OF);
    }
  }
  return data;
}

template <typename type>
type Cpu::Rcr(type data, type count) {
  type masked_count;
  type msb;
  if (sizeof(data) == 1) {
    masked_count = count % 8;
    msb = 0x80;
  } else if (sizeof(data) == 2) {
    masked_count = count % 16;
    msb = 0x8000;
  } else if (sizeof(data) == 4) {
    masked_count = count % 32;
    msb = 0x80000000;
  } else {
    this->obj->Error("Not implemented: sizeof(data)\n", sizeof(data));
  }
  if (masked_count == 1) {
    if (((data & msb) != 0x00) ^ (this->IsFlag(CF))) {
      this->SetFlag(OF);
    } else {
      this->ClearFlag(OF);
    }
  }
  for (int i = 0; i < masked_count; i++) {
    bool temp_cf = data & 0x01;
    data = data >> 1;
    if (this->IsFlag(CF)) {
      data = data | msb;
    }
    if (temp_cf) {
      this->SetFlag(CF);
    } else {
      this->ClearFlag(CF);
    }
  }
  return data;
}

template <typename type>
type Cpu::Rcl(type data, type count) {
  type masked_count;
  type msb;
  if (sizeof(data) == 1) {
    msb = 0x80;
    masked_count = (count & 0x1F) % 9;
  } else if (sizeof(data) == 2) {
    msb = 0x8000;
    masked_count = (count & 0x1F) % 17;
  } else if (sizeof(data) == 4) {
    msb = 0x80000000;
    masked_count = (count & 0x1F);
  } else {
    this->obj->Error("Not implemented: sizeof(data)=%d", sizeof(data));
  }
  for (int i = 0; i < masked_count; i++) {
    bool temp_cf = (data & msb) ? true : false;
    data = (data << 1) + (this->IsFlag(CF) ? 1 : 0);
    if (temp_cf) {
      this->SetFlag(CF);
    } else {
      this->ClearFlag(CF);
    }
  }
  if (masked_count == 1) {
    bool result_msb = (data & msb) ? true : false;
    if (result_msb ^ this->IsFlag(CF)) {
      this->SetFlag(OF);
    } else {
      this->ClearFlag(OF);
    }
  }
  return data;
}

inline template <typename type>
void Cpu::UpdateEflagsForShr(type result) {
  this->UpdateZF((uint32_t)result);
  this->UpdateSF(result);
  this->UpdatePF((uint32_t)result);
}

inline template <typename type>
void Cpu::UpdateOF_Add(type result, type d1, type d2) {
  switch (sizeof(result)) {
    case 1:
      this->eflags.flgs.OF = ((d1 & SIGN_FLG1) == (d2 & SIGN_FLG1)) &&
                             ((result & SIGN_FLG1) != (d1 & SIGN_FLG1));
      break;
    case 2:
      this->eflags.flgs.OF = ((d1 & SIGN_FLG2) == (d2 & SIGN_FLG2)) &&
                             ((result & SIGN_FLG2) != (d1 & SIGN_FLG2));
      break;
    case 4:
      this->eflags.flgs.OF = ((d1 & SIGN_FLG4) == (d2 & SIGN_FLG4)) &&
                             ((result & SIGN_FLG4) != (d1 & SIGN_FLG4));
      break;
    default:
      this->obj->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF",
                       sizeof(result));
  }
}

inline template <typename type>
void Cpu::UpdateCfForSub(type data, int group) {
  switch (group) {
    case 1:
      this->eflags.flgs.CF = ((data >> 8) & 1) ? 1 : 0;
      break;
    case 2:
      this->eflags.flgs.CF = ((data >> 16) & 1) ? 1 : 0;
      break;
    case 4:
    case 8:
      this->eflags.flgs.CF = ((data >> 32) & 1) ? 1 : 0;
      break;
    default:
      this->obj->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF",
                       sizeof(data));
  }
}

inline template <typename type>
void Cpu::UpdateSF(type data) {
  switch (sizeof(data)) {
    case 1:
      this->eflags.flgs.SF = ((data & SIGN_FLG1) == SIGN_FLG1) ? 1 : 0;
      break;
    case 2:
      this->eflags.flgs.SF = ((data & SIGN_FLG2) == SIGN_FLG2) ? 1 : 0;
      break;
    case 4:
    case 8:
      this->eflags.flgs.SF = ((data & SIGN_FLG4) == SIGN_FLG4) ? 1 : 0;
      break;
    default:
      this->obj->Error("Not implemented: data_size=%dbyte at Cpu::UpdateSF",
                       sizeof(data));
  }
}

inline template <typename type>
void Cpu::UpdateEflagsForAnd(type data) {
  this->ClearFlag(CF);
  this->ClearFlag(OF);
  this->UpdateSF(data);
  this->UpdateZF(data);
  this->UpdatePF(data);
}

inline template <typename type>
void Cpu::UpdateEflagsForUnsignedMul(type data) {
  if (data == 0) {
    this->ClearFlag(OF);
    this->ClearFlag(CF);
  } else {
    this->SetFlag(OF);
    this->SetFlag(CF);
  }
}

inline bool Cpu::IsException() { return this->is_exception_; }

inline void Cpu::SetException(int error_code) {
  this->is_exception_ = true;
  this->error_code_ = error_code;
}

inline void Cpu::SetVectorNumber(int vector_number) {
  this->vector_number_ = vector_number;
}

inline int Cpu::GetVectorNumber() { return this->vector_number_; }

inline void Cpu::SetPrefix66() { this->prefix_flgs[FLG_66] = true; }

inline uint8_t Cpu::GetIopl() { return this->eflags.flgs.IOPL; }