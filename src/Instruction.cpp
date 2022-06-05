#include "Instruction.h"
#include "Cpu.h"
#include "Instructions.h"
#include "Memory.h"

using namespace std;
using namespace chrono;

// Instructionクラスのメンバにアクセスすることがない
// or
// Instructionクラスのpublicメンバにしかアクセスしない関数をInstructionHelper空間に移動させた。
// TODO :
// InstructionHelper::ShowInstructionNameを外部からもアクセスできるようにする。
namespace InstructionHelper {
void ShowInstructionName(const Instruction& instruction) {
  cout << instruction.GetInstructionName() << endl;
}
}  // namespace InstructionHelper

unique_ptr<Instruction> Instruction::CreateInstruction(const uint8_t op_code) {
  switch (op_code) {
    case 0x00:
      return make_unique<AddRm8R8>("AddRm8R8");
    case 0x01:
      return make_unique<AddRm32R32>("AddRm32R32");
    case 0x02:
      return make_unique<AddR8Rm8>("AddR8Rm8");
    case 0x03:
      return make_unique<AddR32Rm32>("AddR32Rm32");
    case 0x04:
      return make_unique<AddAlImm8>("AddAlImm8");
    case 0x05:
      return make_unique<AddEaxImm32>("AddEaxImm32");
    case 0x06:
      return make_unique<PushSegmentRegister>("PushSegmentRegister");
    case 0x07:
      return make_unique<PopSegmentRegister>("PopSegmentRegister");
    case 0x08:
      return make_unique<OrRm8R8>("OrRm8R8");
    case 0x09:
      return make_unique<OrRm32R32>("OrRm32R32");
    case 0x0A:
      return make_unique<OrR8Rm8>("OrR8Rm8");
    case 0x0B:
      return make_unique<OrR32Rm32>("OrR32Rm32");
    case 0x0C:
      return make_unique<OrAlImm8>("OrAlImm8");
    case 0x0D:
      return make_unique<OrEaxImm32>("OrEaxImm32");
    case 0x0E:
      return make_unique<PushSegmentRegister>("PushSegmentRegister");
    case 0x0F:
      return make_unique<Code0F>("Code0F");
    case 0x10:
      return make_unique<AdcRm8R8>("AdcRm8R8");
    case 0x11:
      return make_unique<AdcRm32R32>("AdcRm32R32");
    case 0x13:
      return make_unique<AdcR32Rm32>("AdcR32Rm32");
    case 0x15:
      return make_unique<AdcEaxImm32>("AdcEaxImm32");
    case 0x16:
      return make_unique<PushSegmentRegister>("PushSegmentRegister");
    case 0x17:
      return make_unique<PopSegmentRegister>("PopSegmentRegister");
    case 0x18:
      return make_unique<SbbRm8R8>("SbbRm8R8");
    case 0x19:
      return make_unique<SbbRm32R32>("SbbRm32R32");
    case 0x1A:
      return make_unique<SbbR8Rm8>("SbbR8Rm8");
    case 0x1B:
      return make_unique<SbbR32Rm32>("SbbR32Rm32");
    case 0x1E:
      return make_unique<PushSegmentRegister>("PushSegmentRegister");
    case 0x1F:
      return make_unique<PopSegmentRegister>("PopSegmentRegister");
    case 0x20:
      return make_unique<AndRm8R8>("AndRm8R8");
    case 0x21:
      return make_unique<AndRm32R32>("AndRm32R32");
    case 0x22:
      return make_unique<AndR8Rm8>("AndR8Rm8");
    case 0x23:
      return make_unique<AndR32Rm32>("AndR32Rm32");
    case 0x24:
      return make_unique<AndAlImm8>("AndAlImm8");
    case 0x25:
      return make_unique<AndEaxImm32>("AndEaxImm32");
    case 0x28:
      return make_unique<SubRm8R8>("SubRm8R8");
    case 0x29:
      return make_unique<SubRm32R32>("SubRm32R32");
    case 0x2A:
      return make_unique<SubR8Rm8>("SubR8Rm8");
    case 0x2B:
      return make_unique<SubR32Rm32>("SubR32Rm32");
    case 0x2C:
      return make_unique<SubAlImm8>("SubAlImm8");
    case 0x2D:
      return make_unique<SubEaxImm32>("SubEaxImm32");
    case 0x30:
      return make_unique<XorRm8R8>("XorRm8R8");
    case 0x31:
      return make_unique<XorRm32R32>("XorRm32R32");
    case 0x32:
      return make_unique<XorR8Rm8>("XorR8Rm8");
    case 0x33:
      return make_unique<XorR32Rm32>("XorR32Rm32");
    case 0x35:
      return make_unique<XorEaxImm32>("XorEaxImm32");
    case 0x38:
      return make_unique<CmpRm8R8>("CmpRm8R8");
    case 0x39:
      return make_unique<CmpRm32R32>("CmpRm32R32");
    case 0x3A:
      return make_unique<CmpR8Rm8>("CmpR8Rm8");
    case 0x3B:
      return make_unique<CmpR32Rm32>("CmpR32Rm32");
    case 0x3C:
      return make_unique<CmpAlImm8>("CmpAlImm8");
    case 0x3D:
      return make_unique<CmpEaxImm32>("CmpEaxImm32");
    case 0x40 + 0:
    case 0x40 + 1:
    case 0x40 + 2:
    case 0x40 + 3:
    case 0x40 + 4:
    case 0x40 + 5:
    case 0x40 + 6:
    case 0x40 + 7:
      return make_unique<IncR32>("IncR32");
    case 0x48 + 0:
    case 0x48 + 1:
    case 0x48 + 2:
    case 0x48 + 3:
    case 0x48 + 4:
    case 0x48 + 5:
    case 0x48 + 6:
    case 0x48 + 7:
      return make_unique<DecR32>("DecR32");
    case 0x50 + 0:
    case 0x50 + 1:
    case 0x50 + 2:
    case 0x50 + 3:
    case 0x50 + 4:
    case 0x50 + 5:
    case 0x50 + 6:
    case 0x50 + 7:
      return make_unique<PushR32>("PushR32");
    case 0x58 + 0:
    case 0x58 + 1:
    case 0x58 + 2:
    case 0x58 + 3:
    case 0x58 + 4:
    case 0x58 + 5:
    case 0x58 + 6:
    case 0x58 + 7:
      return make_unique<PopR32>("PopR32");
    case 0x60:
      return make_unique<PushAd>("PushAd");
    case 0x61:
      return make_unique<PopAd>("PopAd");
    case 0x68:
      return make_unique<PushImm32>("PushImm32");
    case 0x69:
      return make_unique<ImulR32Rm32Imm32>("ImulR32Rm32Imm32");
    case 0x6A:
      return make_unique<PushImm8>("PushImm8");
    case 0x6B:
      return make_unique<ImulR32Rm32Imm8>("ImulR32Rm32Imm8");
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
      return make_unique<JccRel8>("JccRel8");
    case 0x80:
      return make_unique<Code80>("Code80");
    case 0x81:
      return make_unique<Code81>("Code81");
    case 0x83:
      return make_unique<Code83>("Code83");
    case 0x84:
      return make_unique<TestRm8R8>("TestRm8R8");
    case 0x85:
      return make_unique<TestRm32R32>("TestRm32R32");
    case 0x86:
      return make_unique<XchgRm8R8>("XchgRm8R8");
    case 0x87:
      return make_unique<XchgR32Rm32>("XchgR32Rm32");
    case 0x88:
      return make_unique<MovRm8R8>("MovRm8R8");
    case 0x89:
      return make_unique<MovRm32R32>("MovRm32R32");
    case 0x8A:
      return make_unique<MovR8Rm8>("MovR8Rm8");
    case 0x8B:
      return make_unique<MovR32Rm32>("MovR32Rm32");
    case 0x8C:
      return make_unique<MovRm16Sreg>("MovRm16Sreg");
    case 0x8D:
      return make_unique<LeaR32M>("LeaR32M");
    case 0x8E:
      return make_unique<MovSregRm16>("MovSregRm16");
    case 0x8F:
      return make_unique<PopM32>("PopM32");
    case 0x90:
      return make_unique<Nop>("Nop");
    case 0x90 + 1:  // 1から始める理由は0x90にはNopを割り当てているから。
    case 0x90 + 2:
    case 0x90 + 3:
    case 0x90 + 4:
    case 0x90 + 5:
    case 0x90 + 6:
    case 0x90 + 7:
      return make_unique<XchgEaxR32>("XchgEaxR32");
    case 0x98:
      return make_unique<Cwde>("Cwde");
    case 0x99:
      return make_unique<Cdq>("Cdq");
    case 0x9A:
      return make_unique<CallPtr1632>("CallPtr1632");
    case 0x9C:
      return make_unique<PushFd>("PushFd");
    case 0x9D:
      return make_unique<PopFd>("PopFd");
    case 0x9E:
      return make_unique<Sahf>("Sahf");
    case 0x9F:
      return make_unique<Lahf>("Lahf");
    case 0xA0:
      return make_unique<MovAlMoffs8>("MovAlMoffs8");
    case 0xA1:
      return make_unique<MovEaxMoffs32>("MovEaxMoffs32");
    case 0xA2:
      return make_unique<MovMoffs8Al>("MovMoffs8Al");
    case 0xA3:
      return make_unique<MovMoffs32Eax>("MovMoffs32Eax");
    case 0xA4:
      return make_unique<MovsM8M8>("MovsM8M8");
    case 0xA5:
      return make_unique<MovM32M32>("MovM32M32");
    case 0xA6:
      return make_unique<CmpsM8M8>("CmpsM8M8");
    case 0xA7:
      return make_unique<CmpsM32M32>("CmpsM32M32");
    case 0xA8:
      return make_unique<TestAlImm8>("TestAlImm8");
    case 0xA9:
      return make_unique<TestEaxImm32>("TestEaxImm32");
    case 0xAA:
      return make_unique<StosM8>("StosM8");
    case 0xAB:
      return make_unique<StosM32>("StosM32");
    case 0xAC:
      return make_unique<LodsM8>("LodsM8");
    case 0xAD:
      return make_unique<LodsM32>("LodsM32");
    case 0xAE:
      return make_unique<ScasM8>("ScasM8");
    case 0xAF:
      return make_unique<ScasD>("ScasD");
    case 0xB0 + 0:
    case 0xB0 + 1:
    case 0xB0 + 2:
    case 0xB0 + 3:
    case 0xB0 + 4:
    case 0xB0 + 5:
    case 0xB0 + 6:
    case 0xB0 + 7:
      return make_unique<MovR8Imm8>("MovR8Imm8");
    case 0xB8 + 0:
    case 0xB8 + 1:
    case 0xB8 + 2:
    case 0xB8 + 3:
    case 0xB8 + 4:
    case 0xB8 + 5:
    case 0xB8 + 6:
    case 0xB8 + 7:
      return make_unique<MovR32Imm32>("MovR32Imm32");
    case 0xC0:
      return make_unique<CodeC0>("CodeC0");
    case 0xC1:
      return make_unique<CodeC1>("CodeC1");
    case 0xC2:
      return make_unique<RetImm16>("RetImm16");
    case 0xC3:
      return make_unique<Ret32Near>("Ret32Near");
    case 0xC4:
      return make_unique<LesR32M1632>("LesR32M1632");
    case 0xC5:
      return make_unique<LdsR32M1632>("LdsR32M1632");
    case 0xC6:
      return make_unique<CodeC6>("CodeC6");
    case 0xC7:
      return make_unique<MovRm32Imm32>("MovRm32Imm32");
    case 0xC9:
      return make_unique<Leave>("Leave");
    case 0xCA:
      return make_unique<RetFarImm16>("RetFarImm16");
    case 0xCB:
      return make_unique<Ret32Far>("Ret32Far");
    case 0xCD:
      return make_unique<IntImm8>("IntImm8");
    case 0xCF:
      return make_unique<Iretd>("Iretd");
    case 0xD0:
      return make_unique<CodeD0>("CodeD0");
    case 0xD1:
      return make_unique<CodeD1>("CodeD1");
    case 0xD2:
      return make_unique<CodeD2>("CodeD2");
    case 0xD3:
      return make_unique<CodeD3>("CodeD3");
    case 0xD7:
      return make_unique<Xlatb>("Xlatb");
    case 0xE0:
      return make_unique<LoopneRel8>("LoopneRel8");
    case 0xE1:
      return make_unique<LoopeRel8>("LoopeRel8");
    case 0xE2:
      return make_unique<LoopRel8>("LoopRel8");
    case 0xE3:
      return make_unique<JcxzRel8>("JcxzRel8");
    case 0xE4:
      return make_unique<InAlImm8>("InAlImm8");
    case 0xE6:
      return make_unique<OutImm8>("OutImm8");
    case 0xE8:
      return make_unique<CallRel32>("CallRel32");
    case 0xE9:
      return make_unique<JmpRel32>("JmpRel32");
    case 0xEA:
      return make_unique<JmpPtr1632>("JmpPtr1632");
    case 0xEB:
      return make_unique<JmpRel8>("JmpRel8");
    case 0xEC:
      return make_unique<InAlDx>("InAlDx");
    case 0xEE:
      return make_unique<OutDxAl>("OutDxAl");
    case 0xF2:
      return make_unique<CodeF2>("CodeF2");
    case 0xF3:
      return make_unique<CodeF3>("CodeF3");
    case 0xF4:
      return make_unique<Hlt>("Hlt");
    case 0xF6:
      return make_unique<CodeF6>("CodeF6");
    case 0xF7:
      return make_unique<CodeF7>("CodeF7");
    case 0xF8:
      return make_unique<Clc>("Clc");
    case 0xF9:
      return make_unique<Stc>("Stc");
    case 0xFA:
      return make_unique<Cli>("Cli");
    case 0xFB:
      return make_unique<Sti>("Sti");
    case 0xFC:
      return make_unique<Cld>("Cld");
    case 0xFD:
      return make_unique<Std>("Std");
    case 0xFE:
      return make_unique<CodeFE>("CodeFE");
    case 0xFF:
      return make_unique<CodeFF>("CodeFF");
  }
  unique_ptr<Instruction> p = NULL;
  return p;
}

Instruction::Instruction(string code_name) {
  this->code_name = code_name;
  this->obj = make_unique<Object>();
}

Instruction::~Instruction() {}

inline void Instruction::ParseModRM(Cpu& cpu, Memory& memory) {
  uint8_t code = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  this->modrm.mod = ((code & 0xC0) >> 6);
  this->modrm.op_code = ((code & 0x38) >> 3);
  this->modrm.rm = code & 0x07;
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    if ((this->modrm.mod == 0x01 && this->modrm.rm == 0x05) ||
        (this->modrm.mod == 0x10 && this->modrm.rm == 0x05)) {
      if (!cpu.IsSegmentOverride()) cpu.SetDataSelector(SS);
    }
    // SIB判定
    if (this->modrm.mod != 3 && this->modrm.rm == 4) {
      this->modrm.sib = memory.Read8(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(1);
      this->sib.ParseSib(this->modrm.sib, this->modrm.mod);
      if ((this->sib.GetBase() == 5 && this->modrm.mod == 0x00)) {
        this->modrm.disp32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
        cpu.AddEip(4);
        this->sib.SetDisp32(this->modrm.disp32);
      }
    }
    // disp取得disp32は
    if ((this->modrm.mod == 0 && this->modrm.rm == 5) || this->modrm.mod == 2) {
      this->modrm.disp32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(4);
    } else if (this->modrm.mod == 1) {
      this->modrm.disp8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(1);
    }
  } else {
    if ((this->modrm.mod == 0 && this->modrm.rm == 6) || this->modrm.mod == 2) {
      this->modrm.disp16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(2);
    } else if (this->modrm.mod == 1) {
      this->modrm.disp8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(1);
    }
  }
}

inline void Instruction::ParseRegIdx(Cpu& cpu, Memory& memory) {
  uint8_t code = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  this->modrm.mod = ((code & 0xC0) >> 6);
  this->modrm.op_code = ((code & 0x38) >> 3);
  this->modrm.rm = code & 0x07;
  cpu.AddEip(1);
}

inline uint32_t Instruction::GetEffectiveAddr(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    uint32_t addr;
    if (this->modrm.mod != 3 && this->modrm.rm == 4) {
      addr = this->sib.GetAddress(cpu);
    }
    if (this->modrm.mod == 0) {
      if (this->modrm.rm == 5) {
        addr = this->modrm.disp32;
        return addr;
      }
      if (this->modrm.rm == 0x04) {
        return addr;
      }
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
      return addr;
    }
    if (this->modrm.mod == 1) {
      if (this->modrm.rm == 4) {
        uint32_t disp8 = (int32_t)this->modrm.disp8;
        addr = addr + disp8;
        return addr;
      }
      uint32_t disp8 = (int32_t)this->modrm.disp8;
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm) + disp8;
      return addr;
    }
    if (this->modrm.mod == 2) {
      if (this->modrm.rm == 4) {
        uint32_t disp32 = (int32_t)this->modrm.disp32;
        addr = addr + disp32;
        return addr;
      }
      uint32_t disp32 = (int32_t)this->modrm.disp32;
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm) + disp32;
      return addr;
    }
    if (this->modrm.mod == 3) {
      addr = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
      return addr;
    }
  } else {
    uint16_t addr;
    if (this->modrm.mod == 0) {
      if (this->modrm.rm == 6) {
        addr = this->modrm.disp16;
        return this->modrm.disp16;
      }
      addr = this->GetR16ForEffectiveAddr(cpu, memory);
      return addr;
    }
    if (this->modrm.mod == 1) {
      uint16_t disp8 = (int16_t)this->modrm.disp8;
      addr = disp8 + this->GetR16ForEffectiveAddr(cpu, memory);
      return addr;
    }
    if (this->modrm.mod == 2) {
      uint16_t disp16 = this->modrm.disp16;
      addr = disp16 + this->GetR16ForEffectiveAddr(cpu, memory);
      return addr;
    }
    addr = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
    return addr;
  }
}

inline uint16_t Instruction::GetR16ForEffectiveAddr(Cpu& cpu, Memory& memory) {
  uint16_t data = 0;
  switch (this->modrm.rm) {
    uint16_t r1;
    uint16_t r2;
    case 0:
      r1 = (uint16_t)cpu.GetR32(EBX);
      r2 = (uint16_t)cpu.GetR32(ESI);
      data = r1 + r2;
      return data;
    case 1:
      r1 = (uint16_t)cpu.GetR32(EBX);
      r2 = (uint16_t)cpu.GetR32(EDI);
      data = r1 + r2;
      return data;
    case 2:
      if (!cpu.IsSegmentOverride()) cpu.SetDataSelector(SS);
      r1 = (uint16_t)cpu.GetR32(EBP);
      r2 = (uint16_t)cpu.GetR32(ESI);
      data = r1 + r2;
      return data;
    case 3:
      if (!cpu.IsSegmentOverride()) cpu.SetDataSelector(SS);
      r1 = (uint16_t)cpu.GetR32(EBP);
      r2 = (uint16_t)cpu.GetR32(EDI);
      data = r1 + r2;
      return data;
    case 4:
      r1 = (uint16_t)cpu.GetR32(ESI);
      data = r1;
      return data;
    case 5:
      r1 = (uint16_t)cpu.GetR32(EDI);
      data = r1;
      return data;
    case 6:
      if (!cpu.IsSegmentOverride()) cpu.SetDataSelector(SS);
      r1 = (uint16_t)cpu.GetR32(EBP);
      data = r1;
      return data;
    case 7:
      r1 = (uint16_t)cpu.GetR32(EBX);
      data = r1;
      return data;
  }
  return data;
}

inline uint8_t Instruction::GetRM8(Cpu& cpu, Memory& memory) {
  if (this->modrm.mod != 3) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    uint32_t linear_addr = cpu.GetLinearAddrForDataAccess(effective_addr);
    return memory.Read8(linear_addr);
  }
  return cpu.GetR8(this->modrm.rm);
}

inline uint16_t Instruction::GetRM16(Cpu& cpu, Memory& memory) {
  if (this->modrm.mod != 3) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    uint32_t linear_addr = cpu.GetLinearAddrForDataAccess(effective_addr);
    return memory.Read16(linear_addr);
  }
  return cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
}

inline uint32_t Instruction::GetRM32(Cpu& cpu, Memory& memory) {
  if (this->modrm.mod != 3) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    uint32_t linear_addr = cpu.GetLinearAddrForDataAccess(effective_addr);
    return memory.Read32(linear_addr);
  }
  return cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm);
}

inline void Instruction::SetRM8(Cpu& cpu, Memory& memory, const uint8_t data) {
  if (this->modrm.mod != 3) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    uint32_t linear_addr = cpu.GetLinearAddrForDataAccess(effective_addr);
    memory.Write(linear_addr, data);
    return;
  }
  cpu.SetR8(this->modrm.rm, data);
}

inline void Instruction::SetRM16(Cpu& cpu, Memory& memory,
                                 const uint16_t data) {
  if (this->modrm.mod != 3) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    uint32_t linear_addr = cpu.GetLinearAddrForDataAccess(effective_addr);
    memory.Write(linear_addr, data);
    return;
  }
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
}

inline void Instruction::SetRM32(Cpu& cpu, Memory& memory,
                                 const uint32_t data) {
  if (this->modrm.mod != 3) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    uint32_t linear_addr = cpu.GetLinearAddrForDataAccess(effective_addr);
    memory.Write(linear_addr, data);
    return;
  }
  cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm, data);
  return;
}

JmpRel8::JmpRel8(string code_name) : Instruction(code_name) {}

void JmpRel8::Run(Cpu& cpu, Memory& memory) {
  int32_t diff;
  cpu.AddEip(1);
  diff = (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(diff + 1);
}

MovR32Imm32::MovR32Imm32(string code_name) : Instruction(code_name) {}

void MovR32Imm32::Run(Cpu& cpu, Memory& memory) {
  GENERAL_PURPOSE_REGISTER32 register_type;
  register_type =
      (GENERAL_PURPOSE_REGISTER32)(memory.Read8(
                                       cpu.GetLinearAddrForCodeAccess()) -
                                   0xB8);
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32(register_type, memory.Read32(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(4);
    return;
  }
  cpu.SetR16(register_type, memory.Read16(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(2);
}

MovSregRm16::MovSregRm16(string code_name) : Instruction(code_name) {}

void MovSregRm16::Run(Cpu& cpu, Memory& memory) {
  uint32_t eip = cpu.GetEip();
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  SEGMENT_REGISTER register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
  if (register_type == CS) {
    cpu.SetEip(eip);
    cpu.SetException(-1);
    cpu.SetVectorNumber(CpuEnum::UD);
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  if (cpu.IsProtectedMode() &&
      ((rm16 & 0xFFFC) / 8 + 1) > ((cpu.GetGdtLimit() + 1) / 8)) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  GdtGate* gdt_gate = cpu.GetGdtGate(rm16);
  if (cpu.IsProtectedMode() && (rm16 != 0) &&
      ((gdt_gate->access_right & CpuEnum::S) == 0)) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  if (cpu.IsProtectedMode() && (rm16 != 0) &&
      ((gdt_gate->access_right & CpuEnum::P) == 0)) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    if (register_type != SS) {
      cpu.SetVectorNumber(CpuEnum::NP);
    } else {
      cpu.SetVectorNumber(CpuEnum::SS_VECTOR);
    }
    return;
  }
  if (cpu.IsProtectedMode() && rm16 == 0 && register_type == SS) {
    cpu.SetEip(eip);
    cpu.SetException(0);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  uint8_t dpl = CpuHelper::GetDpl(gdt_gate->access_right);
  uint8_t rpl = CpuHelper::GetRpl(rm16);
  uint8_t cpl = cpu.GetCpl();
  if (cpu.IsProtectedMode() && cpl > dpl && rpl > dpl) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  if (cpu.IsProtectedMode() && (register_type == SS) && (cpl != rpl)) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  if (cpu.IsProtectedMode() && (register_type == SS) &&
      ((gdt_gate->access_right & 0x000F) == CpuEnum::READ_AND_ACCESS)) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  if (cpu.IsProtectedMode() && (register_type == SS) &&
      ((gdt_gate->access_right & 0x000F) == CpuEnum::READ_ONLY)) {
    cpu.SetEip(eip);
    cpu.SetException(rm16 & 0xFFFC);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  if (cpu.IsProtectedMode()) {
    cpu.SetR16(register_type, rm16 & 0xFFFC);
  } else {
    cpu.SetR16(register_type, rm16);
  }
}

MovR8Rm8::MovR8Rm8(string code_name) : Instruction(code_name) {}

void MovR8Rm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8;
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  rm8 = this->GetRM8(cpu, memory);
  cpu.SetR8(this->modrm.reg_index, rm8);
  return;
}

CodeC0::CodeC0(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[4] = make_unique<SalRm8Imm8>("SalRm8Imm8");
  this->instructions[5] = make_unique<ShrRm8Imm8>("ShrRm8Imm8");
  this->instructions[7] = make_unique<SarRm8Imm8>("SarRm8Imm8");
}

void CodeC0::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("code C0 /%02X is not implemented %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeC6::CodeC6(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<MovRm8Imm8>("MovRm8Imm8");
}

void CodeC6::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: C6 %02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

Code80::Code80(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<AddRm8Imm8>("AddRm8Imm8");
  this->instructions[1] = make_unique<OrRm8Imm8>("OrRm8Imm8");
  this->instructions[2] = make_unique<AdcRm8Imm8>("AdcRm8Imm8");
  this->instructions[4] = make_unique<AndRm8Imm8>("AndRm8Imm8");
  this->instructions[5] = make_unique<SubRm8Imm8>("SubRm8Imm8");
  this->instructions[6] = make_unique<XorRm8Imm8>("XorRm8Imm8");
  this->instructions[7] = make_unique<CmpRm8Imm8>("CmpRm8Imm8");
}

void Code80::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: 80 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

Code81::Code81(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<AddRm32Imm32>("AddRm32Imm32");
  this->instructions[1] = make_unique<OrRm32Imm32>("OrRm32Imm32");
  this->instructions[4] = make_unique<AndRm32Imm32>("AndRm32Imm32");
  this->instructions[5] = make_unique<SubRm32Imm32>("SubRm32Imm32");
  this->instructions[6] = make_unique<XorRm32Imm32>("XorRm32Imm32");
  this->instructions[7] = make_unique<CmpRm32Imm32>("CmpRm32Imm32");
}

void Code81::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: 81 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

Code0F::Code0F(string code_name) : Instruction(code_name) {
  for (int i = 0; i < 256; i++) {
    this->instructions[i] = NULL;
  }
  // TODO : 生ポインタをスマートポインタに変更
  this->instructions[0x00] = make_unique<Code0F00>("Code0F00");
  this->instructions[0x01] = make_unique<Code0F01>("Code0F01");
  this->instructions[0x20] = make_unique<MovR32CRX>("MovR32CRX");
  this->instructions[0x22] = make_unique<MovCRXR32>("MovCRXR32");
  this->instructions[0x80] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x81] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x82] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x83] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x84] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x85] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x86] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x87] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x88] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x89] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x8A] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x8B] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x8C] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x8D] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x8E] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x8F] = make_unique<JccRel32>("JccRel32");
  this->instructions[0x92] = make_unique<SetbRm8>("SetbRm8");
  this->instructions[0x94] = make_unique<SeteRm8>("SeteRm8");
  this->instructions[0x95] = make_unique<SetneRm8>("SetneRm8");
  this->instructions[0x97] = make_unique<SetaRm8>("SetaRm8");
  this->instructions[0x9D] = make_unique<SetgeRm8>("SetgeRm8");
  this->instructions[0x9F] = make_unique<SetgRm8>("SetgRm8");
  this->instructions[0xA0] =
      make_unique<PushSegmentRegister>("PushSegmentRegister");
  this->instructions[0xA1] =
      make_unique<PopSegmentRegister>("PopSegmentRegister");
  this->instructions[0xA8] =
      make_unique<PushSegmentRegister>("PushSegmentRegister");
  this->instructions[0xA9] =
      make_unique<PopSegmentRegister>("PopSegmentRegister");
  this->instructions[0xAC] = make_unique<ShrdRm32R32Imm8>("ShrdRm32R32Imm8");
  this->instructions[0xAF] = make_unique<ImulR32Rm32>("ImulR32Rm32");
  this->instructions[0xB2] = make_unique<LssR32M1632>("LssR32M1632");
  this->instructions[0xB4] = make_unique<LfsR32M1632>("LfsR32M1632");
  this->instructions[0xB5] = make_unique<LgsR32M1632>("LgsR32M1632");
  this->instructions[0xB6] = make_unique<MovzxR32Rm8>("MovzxR32Rm8");
  this->instructions[0xB7] = make_unique<MovzxR32Rm16>("MovzxR32Rm16");
  this->instructions[0xBE] = make_unique<MovsxR32Rm8>("MovsxR32Rm8");
  this->instructions[0xBF] = make_unique<MovsxR32Rm16>("MovsxR32Rm16");
}

void Code0F::Run(Cpu& cpu, Memory& memory) {
  uint8_t op_code;
  cpu.AddEip(1);
  op_code = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  if (this->instructions[op_code].get() == NULL) {
    this->obj->Error("Not implemented: 0F %02X at %s::Run", op_code,
                     this->code_name.c_str());
  }
  this->instructions[op_code]->Run(cpu, memory);
  return;
}

CodeC1::CodeC1(string code_name) : Instruction(code_name) {
  for (int i = 0; i < 256; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<RolRm32Imm8>("RolRm32Imm8");
  this->instructions[2] = make_unique<RclRm32Imm8>("RclRm32Imm8");
  this->instructions[4] = make_unique<SalRm32Imm8>("SalRm32Imm8");
  this->instructions[5] = make_unique<ShrRm32Imm8>("ShrRm32Imm8");
  this->instructions[7] = make_unique<SarRm32Imm8>("SarRm32Imm8");
}

void CodeC1::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: C1 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

Code0F00::Code0F00(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[2] = make_unique<LldtRm16>("LldtRm16");
  this->instructions[3] = make_unique<LtrRm16>("LtrRm16");
}

void Code0F00::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: 0F 00 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

Code0F01::Code0F01(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0x02] = make_unique<Lgdt>("Lgdt");
  this->instructions[0x03] = make_unique<Lidt>("Lidt");
}

void Code0F01::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: 0F 01 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

Code83::Code83(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[1] = make_unique<OrRm32Imm8>("OrRm32Imm8");
  this->instructions[0] = make_unique<AddRm32Imm8>("AddRm32Imm8");
  this->instructions[2] = make_unique<AdcRm32Imm8>("AdcRm32Imm8");
  this->instructions[3] = make_unique<SbbRm32Imm8>("SbbRm32Imm8");
  this->instructions[4] = make_unique<AndRm32Imm8>("AndRm32Imm8");
  this->instructions[5] = make_unique<SubRm32Imm8>("SubRm32Imm8");
  this->instructions[6] = make_unique<XorRm32Imm8>("XorRm32Imm8");
  this->instructions[7] = make_unique<CmpRm32Imm8>("CmpRm32Imm8");
}

void Code83::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("code 83 /%02X is not implemented %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeF7::CodeF7(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<TestRm32Imm32>("TestRm32Imm32");
  this->instructions[2] = make_unique<NotRm32>("NotRm32");
  this->instructions[3] = make_unique<NegRm32>("NegRm32");
  this->instructions[4] = make_unique<MulRm32>("MulRm32");
  this->instructions[5] = make_unique<ImulRm16>("ImulRm16");
  this->instructions[6] = make_unique<DivRm32>("DivRm32");
  this->instructions[7] = make_unique<IdivRm32>("IdivRm32");
}

void CodeF7::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: F7 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeFE::CodeFE(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<IncRm8>("IncRm8");
  this->instructions[1] = make_unique<DecRm8>("DecRm8");
}

void CodeFE::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: FE /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeD0::CodeD0(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[1] = make_unique<RorRm8>("RorRm8");
  this->instructions[4] = make_unique<SalRm8>("SalRm8");
  this->instructions[5] = make_unique<ShrRm8>("ShrRm8");
}

void CodeD0::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: D0 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeD2::CodeD2(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[4] = make_unique<SalRm8Cl>("SalRm8Cl");
}

void CodeD2::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: D2 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeFF::CodeFF(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<IncRm32>("IncRm32");
  this->instructions[1] = make_unique<DecRm32>("DecRm32");
  this->instructions[2] = make_unique<CallRm32>("CallRm32");
  this->instructions[3] = make_unique<CallM1632>("CallM1632");
  this->instructions[4] = make_unique<JmpRm32>("JmpRm32");
  this->instructions[5] = make_unique<JmpM1632>("JmpM1632");
  this->instructions[6] = make_unique<PushRm32>("PushRm32");
}

void CodeFF::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: FF /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeD1::CodeD1(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[2] = make_unique<RclRm32>("RclRm32");
  this->instructions[3] = make_unique<RcrRm32>("RcrRm32");
  this->instructions[4] = make_unique<SalRm32>("SalRm32");
  this->instructions[5] = make_unique<ShrRm32>("ShrRm32");
  this->instructions[7] = make_unique<SarRm32>("SarRm32");
}

void CodeD1::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("code D1 /%02X is not implemented %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeD3::CodeD3(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[4] = make_unique<SalRm32Cl>("SalRm32Cl");
  this->instructions[5] = make_unique<ShrRm32Cl>("ShrRm32Cl");
  this->instructions[7] = make_unique<SarRm32Cl>("SarRm32Cl");
}

void CodeD3::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: D3 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

CodeF6::CodeF6(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SET_SMALL_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0] = make_unique<TestRm8Imm8>("TestRm8Imm8");
  this->instructions[2] = make_unique<NotRm8>("NotRm8");
  this->instructions[4] = make_unique<MulRm8>("MulRm8");
  this->instructions[6] = make_unique<DivRm8>("DivRm8");
}

void CodeF6::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (this->instructions[this->modrm.reg_index].get() == NULL) {
    this->obj->Error("Not implemented: F6 /%02X at %s::Run",
                     this->modrm.reg_index, this->code_name.c_str());
  }
  this->instructions[this->modrm.reg_index]->SetModRM(&this->modrm, &this->sib);
  this->instructions[this->modrm.reg_index]->Run(cpu, memory);
  return;
}

AddRm8Imm8::AddRm8Imm8(string code_name) : Instruction(code_name) {}

void AddRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Add(rm8, imm8));
  return;
}

AddRm32Imm8::AddRm32Imm8(string code_name) : Instruction(code_name) {}

// ADD命令のフラグレジスタ更新処理を今後やる。
void AddRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm8_32bits =
        (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(1);
    this->SetRM32(cpu, memory, cpu.Add(rm32, imm8_32bits));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm8 =
      (int16_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  this->SetRM16(cpu, memory, cpu.Add(rm16, imm8));
  cpu.AddEip(1);
  return;
}

AddEaxImm32::AddEaxImm32(string code_name) : Instruction(code_name) {}

void AddEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    uint32_t eax = cpu.GetR32(EAX);
    cpu.SetR32(EAX, cpu.Add(eax, imm32));
    return;
  }
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  uint16_t ax = cpu.GetR16(EAX);
  cpu.SetR16(EAX, cpu.Add(ax, imm16));
  return;
}

AddRm32R32::AddRm32R32(string code_name) : Instruction(code_name) {}

void AddRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint64_t result = rm32 + r32;
    this->SetRM32(cpu, memory, cpu.Add(rm32, r32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM16(cpu, memory, cpu.Add(rm16, r16));
  return;
}

AddR32Rm32::AddR32Rm32(string code_name) : Instruction(code_name) {}

void AddR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               cpu.Add(r32, rm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             cpu.Add(r16, rm16));
  return;
}

CmpAlImm8::CmpAlImm8(string code_name) : Instruction(code_name) {}

void CmpAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  cpu.Sub(al, imm8);
  return;
}

MovR8Imm8::MovR8Imm8(string code_name) : Instruction(code_name) {}

void MovR8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint32_t register_type =
      memory.Read8(cpu.GetLinearAddrForCodeAccess()) - 0xB0;
  cpu.AddEip(1);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  cpu.SetR8(register_type, imm8);
  return;
}

IntImm8::IntImm8(string code_name) : Instruction(code_name) {}

void IntImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint16_t selector = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  if (cpu.IsProtectedMode()) {
    IdtGate* idt_gate = cpu.GetIdtGate(selector * 8);
    uint32_t offset_addr = (((uint32_t)idt_gate->offset_high) << 16) |
                           ((uint32_t)idt_gate->offset_low);
    uint8_t idt_gate_dpl = CpuHelper::GetDpl(idt_gate->access_right);
    uint8_t cpl = cpu.GetCpl();
    GdtGate* gdt_gate = cpu.GetGdtGate(idt_gate->selector);
    uint8_t dest_code_segment_dpl = CpuHelper::GetDpl(gdt_gate->access_right);
    if (idt_gate_dpl < cpl) {
      this->obj->Error("Not implemented: dpl(idt_gate)<cpl at %s::Run",
                       this->code_name.c_str());
    }
    if (dest_code_segment_dpl < cpl) {
      uint16_t ss = cpu.GetR16(SS);
      uint32_t eflags = cpu.GetEflgs();
      uint32_t esp = cpu.GetR32(ESP);
      uint16_t cs = cpu.GetR16(CS);
      uint32_t eip = cpu.GetEip();
      Tss* tss = cpu.GetCurrentTss();
      cpu.SetR16(SS, tss->ss0);
      cpu.SetR32(ESP, tss->esp0);
      cpu.Push32(ss);
      cpu.Push32(esp);
      cpu.Push32(eflags);
      cpu.Push32(cs);
      cpu.Push32(eip);
      cpu.SetEip(offset_addr);
      cpu.SetR16(CS, idt_gate->selector);
      // cpu.SetRpl(CS, cpl);
      cpu.ClearFlag(IF);
    } else if (dest_code_segment_dpl == cpl) {
      cpu.Push32(cpu.GetEflgs());
      cpu.Push32(cpu.GetR16(CS));
      cpu.Push32(cpu.GetEip());
      cpu.SetEip(offset_addr);
      cpu.SetR16(CS, idt_gate->selector);
      // cpu.SetRpl(CS, cpl);
      cpu.ClearFlag(IF);
    } else {  // dpl>cpl
      this->obj->Error("Not implemented: dest_code_segment_dpl>cpl at %s::Run",
                       this->code_name.c_str());
    }
    return;
  }
  if (selector < 0x20) {
    cpu.CallFunctionOnRealMode(selector);
  } else {
    //リアルモードではEFLAGSは下位16bitだけでいい。
    //割り込みテーブルでは、IP、CSの順に並んでいる。
    uint16_t eflags = cpu.GetEflgs();
    uint16_t ip = cpu.GetEip();
    uint16_t new_ip, new_cs;
    cpu.Push16(eflags);
    cpu.ClearFlag(IF);
    cpu.ClearFlag(TF);
    cpu.ClearFlag(AC);
    cpu.Push16(cpu.GetR16(CS));
    cpu.Push16(ip);
    new_ip = memory.Read16((selector << 2));
    new_cs = memory.Read16((selector << 2) + 2);
    cpu.SetR16(CS, new_cs);
    cpu.SetEip(new_ip);
  }
  return;
}

Hlt::Hlt(string code_name) : Instruction(code_name) {}

void Hlt::Run(Cpu& cpu, Memory& memory) {
  if (cpu.IsProtectedMode() && cpu.GetCpl() != 0) {
    cpu.SetException(0);
    cpu.SetVectorNumber(CpuEnum::GP);
    return;
  }
  cpu.AddEip(1);
  this_thread::sleep_for(milliseconds(10));
}

MovRm16Sreg::MovRm16Sreg(string code_name) : Instruction(code_name) {}

void MovRm16Sreg::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  SEGMENT_REGISTER register_type = (SEGMENT_REGISTER)this->modrm.reg_index;
  uint16_t register_value = cpu.GetR16(register_type);
  this->SetRM16(cpu, memory, register_value);
  return;
}

CmpRm8Imm8::CmpRm8Imm8(string code_name) : Instruction(code_name) {}

void CmpRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  cpu.Sub(rm8, imm8);
  return;
}

MovRm8R8::MovRm8R8(string code_name) : Instruction(code_name) {}

void MovRm8R8::Run(Cpu& cpu, Memory& memory) {
  uint8_t r8;
  uint8_t rm8;
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  r8 = cpu.GetR8(this->modrm.reg_index);
  this->SetRM8(cpu, memory, r8);
  return;
}

JmpRel32::JmpRel32(string code_name) : Instruction(code_name) {}

void JmpRel32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.AddEip(1);
    int32_t diff = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(diff + 4);
    return;
  }
  cpu.AddEip(1);
  int32_t diff =
      (int32_t)((int16_t)memory.Read16(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(diff + 2);
  return;
}

MovRm8Imm8::MovRm8Imm8(string code_name) : Instruction(code_name) {}

void MovRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8;
  uint8_t imm8;
  GENERAL_PURPOSE_REGISTER32 register_type;
  imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, imm8);
  return;
}

MovRm32Imm32::MovRm32Imm32(string code_name) : Instruction(code_name) {}

void MovRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm32;
    cpu.AddEip(1);
    this->ParseModRM(cpu, memory);
    imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    this->SetRM32(cpu, memory, imm32);
    return;
  }
  uint16_t imm16;
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  this->SetRM16(cpu, memory, imm16);
  return;
}

MovMoffs8Al::MovMoffs8Al(string code_name) : Instruction(code_name) {}

void MovMoffs8Al::Run(Cpu& cpu, Memory& memory) {
  uint8_t al;
  uint32_t addr;
  cpu.AddEip(1);
  al = cpu.GetR8L(EAX);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    uint32_t offset32;
    offset32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    addr = cpu.GetLinearAddrForDataAccess(offset32);
    memory.Write(addr, al);
    return;
  } else {
    uint16_t offset16;
    offset16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(2);
    addr = cpu.GetLinearAddrForDataAccess((uint32_t)offset16);
    memory.Write(addr, al);
  }
  return;
}

OutImm8::OutImm8(string code_name) : Instruction(code_name) {}

void OutImm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t imm8;
  cpu.AddEip(1);
  imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  return;
}

Nop::Nop(string code_name) : Instruction(code_name) {}

void Nop::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  return;
}

Cli::Cli(string code_name) : Instruction(code_name) {}

void Cli::Run(Cpu& cpu, Memory& memory) {
  uint32_t eip = cpu.GetEip();
  cpu.AddEip(1);
  if (cpu.IsRealMode()) {
    cpu.ClearFlag(IF);
    return;
  }
  if (cpu.GetIopl() >= cpu.GetCpl()) {
    cpu.ClearFlag(IF);
  } else {
    cpu.SetEip(eip);
    cpu.SetException(0);
    cpu.SetVectorNumber(CpuEnum::GP);
  }
  return;
}

CallRel32::CallRel32(string code_name) : Instruction(code_name) {}

void CallRel32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rel32;
    rel32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.Push32(cpu.GetEip() + 4);
    cpu.AddEip(rel32 + 4);
    return;
  }
  uint16_t rel16;
  rel16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.Push16(cpu.GetEip() + 2);
  cpu.AddEip(rel16 + 2);
  return;
}

InAlImm8::InAlImm8(string code_name) : Instruction(code_name) {}

void InAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsProtectedMode()) {
    this->obj->Error("Not implemented: protected mode at %s::Run",
                     this->code_name.c_str());
    return;
  }
  uint8_t imm8;
  imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  cpu.SetR8L(EAX, this->io_in8(imm8));
  return;
}

AndAlImm8::AndAlImm8(string code_name) : Instruction(code_name) {}

void AndAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  cpu.SetR8L(EAX, cpu.And(cpu.GetR8L(EAX), imm8));
  return;
}

Ret32Near::Ret32Near(string code_name) : Instruction(code_name) {}

void Ret32Near::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetEip(cpu.Pop32());
    return;
  }
  uint32_t addr;
  addr = 0x0000FFFF & cpu.Pop16();
  cpu.SetEip(addr);
  return;
}

Lgdt::Lgdt(string code_name) : Instruction(code_name) {}

void Lgdt::Run(Cpu& cpu, Memory& memory) {
  uint16_t limit;
  uint32_t base_addr;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t m32;
    m32 = this->GetEffectiveAddr(cpu, memory);
    limit = memory.Read16(cpu.GetLinearAddrForDataAccess(m32));
    base_addr = memory.Read32(cpu.GetLinearAddrForDataAccess(m32 + 2));
    cpu.SetGdtr(limit, base_addr);
    return;
  }
  uint16_t effective_addr;
  effective_addr = this->GetEffectiveAddr(cpu, memory);
  limit = memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr));
  base_addr = 0x00FFFFFF &
              memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr + 2));
  cpu.SetGdtr(limit, base_addr);
  return;
}

Lidt::Lidt(string code_name) : Instruction(code_name) {}

void Lidt::Run(Cpu& cpu, Memory& memory) {
  uint16_t limit;
  uint32_t m32, base;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    m32 = this->GetEffectiveAddr(cpu, memory);
    limit = memory.Read16(cpu.GetLinearAddrForDataAccess(m32));
    base = memory.Read32(cpu.GetLinearAddrForDataAccess(m32 + 2));
    cpu.SetIdtr(limit, base);
  } else {
    this->obj->Error("Not implemented: %s::Run");
  }
  return;
}
MovR32CRX::MovR32CRX(string code_name) : Instruction(code_name) {}

void MovR32CRX::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseRegIdx(cpu, memory);
  cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm,
             cpu.GetCr((CONTROL_REGISTER)this->modrm.reg_index));
  return;
}

AndEaxImm32::AndEaxImm32(string code_name) : Instruction(code_name) {}

void AndEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    uint32_t eax = cpu.GetR32(EAX);
    cpu.SetR32(EAX, cpu.And(eax, imm32));
    return;
  }
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  uint16_t ax = cpu.GetR16(EAX);
  cpu.SetR16(EAX, cpu.And(ax, imm16));
  return;
}

OrRm32Imm8::OrRm32Imm8(string code_name) : Instruction(code_name) {}

void OrRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm8 =
        (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(1);
    this->SetRM32(cpu, memory, cpu.Or(rm32, imm8));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm8 =
      (int16_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  this->SetRM16(cpu, memory, cpu.Or(rm16, imm8));
  return;
}

MovCRXR32::MovCRXR32(string code_name) : Instruction(code_name) {}

void MovCRXR32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseRegIdx(cpu, memory);
  cpu.SetCr((CONTROL_REGISTER)this->modrm.reg_index,
            cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.rm));
  return;
}

MovzxR32Rm8::MovzxR32Rm8(string code_name) : Instruction(code_name) {}

void MovzxR32Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               (uint32_t)((uint8_t)this->GetRM8(cpu, memory)));
    return;
  }
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             (uint16_t)((uint8_t)this->GetRM8(cpu, memory)));
  return;
}

MovR32Rm32::MovR32Rm32(string code_name) : Instruction(code_name) {}

void MovR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               this->GetRM32(cpu, memory));
    return;
  }
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             this->GetRM16(cpu, memory));
  return;
}

MovRm32R32::MovRm32R32(string code_name) : Instruction(code_name) {}

void MovRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->SetRM32(
        cpu, memory,
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index));
    return;
  }
  this->SetRM16(cpu, memory,
                cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index));
  return;
}

SubRm32Imm8::SubRm32Imm8(string code_name) : Instruction(code_name) {}

void SubRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm8 =
        (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(1);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Sub(rm32, imm8));
    return;
  }
  uint16_t imm8 =
      (int16_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Sub(rm16, imm8));
  return;
}

ImulR32Rm32Imm32::ImulR32Rm32Imm32(string code_name) : Instruction(code_name) {}

//フラグレジスタの更新が未実装
void ImulR32Rm32Imm32::Run(Cpu& cpu, Memory& memory) {
  uint32_t rm32;
  uint32_t imm32;
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    rm32 = this->GetRM32(cpu, memory);
    imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32 * imm32);
    return;
  }
  this->obj->Error("Not implemented: 16bit op_size at %s::Run",
                   this->code_name.c_str());
  return;
}

SubRm32Imm32::SubRm32Imm32(string code_name) : Instruction(code_name) {}

//フラグレジスタの更新が未実装
void SubRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    this->SetRM32(cpu, memory, cpu.Sub(rm32, imm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  this->SetRM16(cpu, memory, cpu.Sub(rm16, imm16));
  return;
}

ShrRm32Imm8::ShrRm32Imm8(string code_name) : Instruction(code_name) {}

void ShrRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(1);
    this->SetRM32(cpu, memory, cpu.Shr(rm32, imm8));
    return;
  }
  this->obj->Error("Not implemented: 16bit op_size at %s::Run",
                   this->code_name.c_str());
}

JmpPtr1632::JmpPtr1632(string code_name) : Instruction(code_name) {}

void JmpPtr1632::Run(Cpu& cpu, Memory& memory) {
  GdtGate* gdt_gate;
  if (cpu.IsProtectedMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      uint32_t offset;
      uint16_t selector;
      cpu.AddEip(1);
      offset = memory.Read32(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(4);
      selector = memory.Read16(cpu.GetLinearAddrForCodeAccess());
      gdt_gate = cpu.GetGdtGate(selector);
      if ((gdt_gate->access_right & 0x1D) == TSS_TYPE) {
        cpu.SaveTask(selector);
        cpu.SwitchTask();
        return;
      }
      cpu.SetR16(CS, selector);
      cpu.SetEip(offset);
      return;
    }
    uint16_t offset;
    uint16_t selector;
    cpu.AddEip(1);
    offset = memory.Read16(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(2);
    selector = memory.Read16(cpu.GetLinearAddrForCodeAccess());
    gdt_gate = cpu.GetGdtGate(selector);
    if ((gdt_gate->access_right & 0x1D) == TSS_TYPE) {
      cpu.SaveTask(selector);
      cpu.SwitchTask();
      return;
    }
    cpu.SetR16(CS, selector);
    cpu.SetEip(offset);
    return;
  }
  //リアルモード処理
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32 at %s::Run",
                     this->code_name.c_str());
    return;
  }
  uint16_t offset;
  uint16_t selector;
  cpu.AddEip(1);
  offset = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  selector = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.SetR16(CS, selector);
  cpu.SetEip(offset);
  return;
}

PushR32::PushR32(string code_name) : Instruction(code_name) {}

void PushR32::Run(Cpu& cpu, Memory& memory) {
  GENERAL_PURPOSE_REGISTER32 register_type;
  register_type =
      (GENERAL_PURPOSE_REGISTER32)((uint32_t)memory.Read8(
                                       cpu.GetLinearAddrForCodeAccess()) -
                                   (uint32_t)0x50);
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.Push32(cpu.GetR32(register_type));
    return;
  }
  cpu.Push16(cpu.GetR16(register_type));
  return;
}

PopR32::PopR32(string code_name) : Instruction(code_name) {}

void PopR32::Run(Cpu& cpu, Memory& memory) {
  GENERAL_PURPOSE_REGISTER32 register_type;
  register_type =
      (GENERAL_PURPOSE_REGISTER32)((uint32_t)memory.Read8(
                                       cpu.GetLinearAddrForCodeAccess()) -
                                   (uint32_t)0x58);
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32(register_type, cpu.Pop32());
    return;
  }
  cpu.SetR16(register_type, cpu.Pop16());
  return;
}

PushImm8::PushImm8(string code_name) : Instruction(code_name) {}

void PushImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm8_32bit;
    imm8_32bit =
        (int32_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.Push32(imm8_32bit);
    cpu.AddEip(1);
    return;
  }
  uint16_t imm8_16bit;
  imm8_16bit = (uint16_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.Push16(imm8_16bit);
  cpu.AddEip(1);
  return;
}

IncR32::IncR32(string code_name) : Instruction(code_name) {}

void IncR32::Run(Cpu& cpu, Memory& memory) {
  GENERAL_PURPOSE_REGISTER32 register_type =
      (GENERAL_PURPOSE_REGISTER32)((uint32_t)memory.Read8(
                                       cpu.GetLinearAddrForCodeAccess()) -
                                   (uint32_t)0x40);
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 = cpu.GetR32(register_type);
    cpu.SetR32(register_type, cpu.Inc(r32));
    return;
  }
  uint16_t r16 = cpu.GetR16(register_type);
  cpu.SetR16(register_type, cpu.Inc(r16));
  return;
}

CmpRm32Imm32::CmpRm32Imm32(string code_name) : Instruction(code_name) {}

void CmpRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.Sub(rm32, imm32);
    return;
  }
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  cpu.Sub(rm16, imm16);
  return;
}

AndRm32Imm8::AndRm32Imm8(string code_name) : Instruction(code_name) {}

void AndRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm8 =
        (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(1);
    this->SetRM32(cpu, memory, cpu.And(imm8, this->GetRM32(cpu, memory)));
    return;
  }
  uint16_t imm8 =
      (int16_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  this->SetRM16(cpu, memory, cpu.And(imm8, this->GetRM16(cpu, memory)));
  return;
}

XorRm32R32::XorRm32R32(string code_name) : Instruction(code_name) {}

void XorRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Xor(rm32, r32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM16(cpu, memory, cpu.Xor(rm16, r16));
  return;
}

RorRm8Cl::RorRm8Cl(string code_name) : Instruction(code_name) {}

void RorRm8Cl::Run(Cpu& cpu, Memory& memory) {
  uint8_t cl = cpu.GetR8L(ECX);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Ror(rm8, cl));
  return;
}

PushImm32::PushImm32(string code_name) : Instruction(code_name) {}

void PushImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.Push32(memory.Read32(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(4);
    return;
  }
  cpu.Push16(memory.Read16(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(2);
  return;
}

PushFd::PushFd(string code_name) : Instruction(code_name) {}

void PushFd::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eflgs = cpu.GetEflgs();
    cpu.Push32(eflgs & 0x00FCFFFF);
    return;
  }
  uint16_t eflgs = cpu.GetEflgs();
  cpu.Push16(eflgs);
  return;
}

OutDxAl::OutDxAl(string code_name) : Instruction(code_name) {}

void OutDxAl::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->io_out8(cpu.GetR16(EDX), cpu.GetR8L(EAX));
  return;
}

CmpRm32R32::CmpRm32R32(string code_name) : Instruction(code_name) {}

void CmpRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.Sub(rm32, r32);
    return;
  }
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  cpu.Sub(rm16, r16);
  return;
}

ShrRm8Imm8::ShrRm8Imm8(string code_name) : Instruction(code_name) {}

void ShrRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.Shr(rm8, imm8));
}

LeaR32M::LeaR32M(string code_name) : Instruction(code_name) {}

//この機械語命令はオペランドサイズとアドレスサイズを考慮する必要があります。
void LeaR32M::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if ((!cpu.Is32bitsMode()) ^ cpu.IsPrefixAddrSize()) {  // 16bit addr_size
      this->obj->Error("Not implemented: addr_size=16 at %s::Run",
                       this->code_name.c_str());
    }
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               effective_addr);
    return;
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {  // 32bit addr_size
    this->obj->Error("Not implemented: addr_size=32 at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, effective_addr);
  return;
}

PopFd::PopFd(string code_name) : Instruction(code_name) {}

void PopFd::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eflgs;
    eflgs = cpu.Pop32();
    cpu.SetEflgs(eflgs);
    return;
  }
  uint32_t eflgs;
  eflgs = cpu.Pop16();
  cpu.SetEflgs((cpu.GetEflgs() & 0xFFFF0000) | eflgs);
  return;
}

Leave::Leave(string code_name) : Instruction(code_name) {}

void Leave::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32(ESP, cpu.GetR32(EBP));
    cpu.SetR32(EBP, cpu.Pop32());
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

CmpR32Rm32::CmpR32Rm32(string code_name) : Instruction(code_name) {}

void CmpR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.Sub(r32, rm32);
    return;
  }
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  cpu.Sub(r16, rm16);
  return;
}

ImulR32Rm32::ImulR32Rm32(string code_name) : Instruction(code_name) {}

void ImulR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32;
    uint32_t r32;
    rm32 = this->GetRM32(cpu, memory);
    r32 = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32 * r32);
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
  return;
}

MovsxR32Rm16::MovsxR32Rm16(string code_name) : Instruction(code_name) {}

void MovsxR32Rm16::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint32_t rm16;
  rm16 = (int32_t)(int16_t)this->GetRM16(cpu, memory);
  cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
  return;
}

PushRm32::PushRm32(string code_name) : Instruction(code_name) {}

void PushRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.Push32(this->GetRM32(cpu, memory));
    return;
  }
  cpu.Push16(this->GetRM16(cpu, memory));
  return;
}

IncRm32::IncRm32(string code_name) : Instruction(code_name) {}

void IncRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Inc(rm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Inc(rm16));
  return;
}

DecR32::DecR32(string code_name) : Instruction(code_name) {}

void DecR32::Run(Cpu& cpu, Memory& memory) {
  GENERAL_PURPOSE_REGISTER32 register_type =
      (GENERAL_PURPOSE_REGISTER32)(memory.Read8(
                                       cpu.GetLinearAddrForCodeAccess()) -
                                   0x48);
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 = cpu.GetR32(register_type);
    cpu.SetR32(register_type, cpu.Dec(r32));
    return;
  }
  uint16_t r16 = cpu.GetR16(register_type);
  cpu.SetR16(register_type, cpu.Dec(r16));
  return;
}

TestRm8R8::TestRm8R8(string code_name) : Instruction(code_name) {}

void TestRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8(this->modrm.reg_index);
  cpu.And(rm8, r8);
  return;
}

CmpRm32Imm8::CmpRm32Imm8(string code_name) : Instruction(code_name) {}

void CmpRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm8 =
        (int32_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(1);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.Sub(rm32, imm8);
    return;
  }
  uint16_t imm8 =
      (int16_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  cpu.Sub(rm16, imm8);
  return;
}

AndRm32Imm32::AndRm32Imm32(string code_name) : Instruction(code_name) {}

void AndRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.And(rm32, imm32));
    return;
  }
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.And(rm16, imm16));
  return;
}

SalRm32Imm8::SalRm32Imm8(string code_name) : Instruction(code_name) {}

void SalRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm8 = (uint32_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(1);
    this->SetRM32(cpu, memory, cpu.Sal(rm32, imm8));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm8 = (uint32_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM16(cpu, memory, cpu.Sal(rm16, imm8));
  return;
}

MovsxR32Rm8::MovsxR32Rm8(string code_name) : Instruction(code_name) {}

void MovsxR32Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm8;
    rm8 = (int32_t)(int8_t)this->GetRM8(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
    return;
  }
  uint16_t rm8;
  rm8 = (int16_t)(int8_t)this->GetRM8(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
  return;
}

AndR8Rm8::AndR8Rm8(string code_name) : Instruction(code_name) {}

void AndR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  cpu.SetR8(
      (GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
      cpu.And(cpu.GetR8(this->modrm.reg_index), this->GetRM8(cpu, memory)));
}

XchgR32Rm32::XchgR32Rm32(string code_name) : Instruction(code_name) {}

void XchgR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32;
    uint32_t r32;
    rm32 = this->GetRM32(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm32);
    return;
  }
  uint32_t rm16;
  uint32_t r16;
  rm16 = this->GetRM16(cpu, memory);
  r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM16(cpu, memory, r16);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
}

TestRm32R32::TestRm32R32(string code_name) : Instruction(code_name) {}

void TestRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    cpu.And(rm32, r32);
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.And(rm16, r16);
  return;
}

DivRm32::DivRm32(string code_name) : Instruction(code_name) {}

void DivRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32;
    uint64_t r64;
    uint32_t edx, eax;
    edx = cpu.GetR32(EDX);
    eax = cpu.GetR32(EAX);
    rm32 = this->GetRM32(cpu, memory);

    r64 = (((uint64_t)edx) << ((uint64_t)32)) | ((uint64_t)eax);
    cpu.SetR32(EAX, r64 / ((uint64_t)rm32));
    cpu.SetR32(EDX, r64 % (uint64_t)rm32);
    return;
  }
  uint16_t rm16;
  uint32_t r32;
  uint16_t dx, ax;
  dx = cpu.GetR16(EDX);
  ax = cpu.GetR16(EAX);
  rm16 = this->GetRM16(cpu, memory);
  r32 = (((uint32_t)dx) << ((uint32_t)16)) | ((uint32_t)ax);
  cpu.SetR16(EAX, r32 / ((uint32_t)rm16));
  cpu.SetR16(EDX, r32 % (uint32_t)rm16);
  return;
}

NotRm32::NotRm32(string code_name) : Instruction(code_name) {}

void NotRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32;
    rm32 = this->GetRM32(cpu, memory);
    rm32 = ~rm32;
    this->SetRM32(cpu, memory, rm32);
    return;
  }
  uint16_t rm16;
  rm16 = this->GetRM16(cpu, memory);
  rm16 = ~rm16;
  this->SetRM16(cpu, memory, rm16);
  return;
}

SubRm32R32::SubRm32R32(string code_name) : Instruction(code_name) {}

void SubRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    this->SetRM32(cpu, memory, cpu.Sub(rm32, r32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM16(cpu, memory, cpu.Sub(rm16, r16));
  return;
}

SarRm32Imm8::SarRm32Imm8(string code_name) : Instruction(code_name) {}

void SarRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm8 = (uint32_t)(memory.Read8(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(1);
    this->SetRM32(cpu, memory, cpu.Sar(rm32, imm8));
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

OrRm32R32::OrRm32R32(string code_name) : Instruction(code_name) {}

void OrRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    this->SetRM32(cpu, memory, cpu.Or(rm32, r32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM16(cpu, memory, cpu.Or(rm16, r16));
  return;
}

Cdq::Cdq(string code_name) : Instruction(code_name) {}

void Cdq::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    bool sign_flg = (cpu.GetR32(EAX) & SIGN_FLG4) ? true : false;
    if (sign_flg) {
      cpu.SetR32(EDX, 0xFFFFFFFF);
    } else {
      cpu.SetR32(EDX, 0x00000000);
    }
    return;
  }
  bool sign_flg = (cpu.GetR16(EAX) & SIGN_FLG2) ? true : false;
  if (sign_flg) {
    cpu.SetR16(EDX, 0xFFFF);
  } else {
    cpu.SetR16(EDX, 0x0000);
  }
  return;
}

IdivRm32::IdivRm32(string code_name) : Instruction(code_name) {}

void IdivRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    int64_t rm32;
    int64_t r64;
    int32_t edx, eax;
    edx = cpu.GetR32(EDX);
    eax = cpu.GetR32(EAX);
    rm32 = (int64_t)(int32_t)this->GetRM32(cpu, memory);
    r64 = (((int64_t)edx) << ((int64_t)32)) | ((int64_t)eax);
    cpu.SetR32(EAX, r64 / rm32);
    cpu.SetR32(EDX, r64 % rm32);
    return;
  }
  int32_t rm16;
  int32_t r32;
  int16_t dx, ax;
  dx = cpu.GetR16(EDX);
  ax = cpu.GetR16(EAX);
  rm16 = (int32_t)(int16_t)this->GetRM16(cpu, memory);
  r32 = (((int32_t)dx) << ((int32_t)32)) | ((int32_t)ax);
  cpu.SetR16(EAX, r32 / rm16);
  cpu.SetR16(EDX, r32 % rm16);
  return;
}

Sti::Sti(string code_name) : Instruction(code_name) {}

void Sti::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.SetFlag(IF);
  return;
}

PushAd::PushAd(string code_name) : Instruction(code_name) {}

void PushAd::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t esp = cpu.GetR32(ESP);
    cpu.Push32(cpu.GetR32(EAX));
    cpu.Push32(cpu.GetR32(ECX));
    cpu.Push32(cpu.GetR32(EDX));
    cpu.Push32(cpu.GetR32(EBX));
    cpu.Push32(esp);
    cpu.Push32(cpu.GetR32(EBP));
    cpu.Push32(cpu.GetR32(ESI));
    cpu.Push32(cpu.GetR32(EDI));
    return;
  }
  uint16_t sp = cpu.GetR16(ESP);
  cpu.Push16(cpu.GetR16(EAX));
  cpu.Push16(cpu.GetR16(ECX));
  cpu.Push16(cpu.GetR16(EDX));
  cpu.Push16(cpu.GetR16(EBX));
  cpu.Push16(sp);
  cpu.Push16(cpu.GetR16(EBP));
  cpu.Push16(cpu.GetR16(ESI));
  cpu.Push16(cpu.GetR16(EDI));
  return;
}

InAlDx::InAlDx(string code_name) : Instruction(code_name) {}

void InAlDx::Run(Cpu& cpu, Memory& memory) {
  uint8_t al;
  uint32_t dx;
  cpu.AddEip(1);
  dx = (uint32_t)cpu.GetR16(EDX);
  cpu.SetR8(EAX, this->io_in8(dx));
  return;
}

DecRm32::DecRm32(string code_name) : Instruction(code_name) {}

void DecRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Dec(rm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Dec(rm16));
  return;
}

PopAd::PopAd(string code_name) : Instruction(code_name) {}

void PopAd::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    edi = cpu.Pop32();
    esi = cpu.Pop32();
    ebp = cpu.Pop32();
    esp = cpu.Pop32();
    ebx = cpu.Pop32();
    edx = cpu.Pop32();
    ecx = cpu.Pop32();
    eax = cpu.Pop32();

    cpu.SetR32(EAX, eax);
    cpu.SetR32(ECX, ecx);
    cpu.SetR32(EDX, edx);
    cpu.SetR32(EBX, ebx);
    // espは無視する
    cpu.SetR32(EBP, ebp);
    cpu.SetR32(ESI, esi);
    cpu.SetR32(EDI, edi);
    return;
  } else {
    uint16_t ax, cx, dx, bx, sp, bp, si, di;
    di = cpu.Pop16();
    si = cpu.Pop16();
    bp = cpu.Pop16();
    sp = cpu.Pop16();
    bx = cpu.Pop16();
    dx = cpu.Pop16();
    cx = cpu.Pop16();
    ax = cpu.Pop16();

    cpu.SetR16(EAX, ax);
    cpu.SetR16(ECX, cx);
    cpu.SetR16(EDX, dx);
    cpu.SetR16(EBX, bx);
    // espは無視する
    cpu.SetR16(EBP, bp);
    cpu.SetR16(ESI, si);
    cpu.SetR16(EDI, di);
  }
  return;
}

Iretd::Iretd(string code_name) : Instruction(code_name) {}

void Iretd::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsRealMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                       this->code_name.c_str());
    } else {
      uint16_t cs, ip, eflags;
      ip = cpu.Pop16();
      cs = cpu.Pop16();
      eflags = cpu.Pop16();
      cpu.SetEip(ip);
      cpu.SetR16(CS, cs);
      cpu.SetEflgs(eflags);
    }
    return;
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    bool outer_privilege_level;
    uint16_t cs, ss;
    uint32_t eip, eflags, esp;
    uint8_t cpl, rpl;
    eip = cpu.Pop32();
    cs = cpu.Pop32();
    eflags = cpu.Pop32();
    rpl = CpuHelper::GetRpl(cs);  //呼び出し元特権レベル
    cpl = cpu.GetCpl();
    if (rpl > cpl) {
      esp = cpu.Pop32();
      ss = cpu.Pop32();
      cpu.SetR32(ESP, esp);
      cpu.SetR16(SS, ss);
      cpu.SetR16(CS, cs);
      cpu.SetEip(eip);
      cpu.SetEflgs(eflags);
      SEGMENT_REGISTER array[] = {ES, FS, GS, DS};
      for (auto segment_register_type : array) {
        uint16_t selector = cpu.GetR16(segment_register_type);
        GdtGate* gdt_gate = cpu.GetGdtGate(selector);
        uint8_t dpl = CpuHelper::GetDpl(gdt_gate->access_right);
        if (cpu.GetCpl() > dpl) {
          cpu.SetR16(segment_register_type, 0);
        }
      }
    } else if (rpl == cpl) {  // rpl<=cpl
      cpu.SetR16(CS, cs);
      cpu.SetEip(eip);
      cpu.SetEflgs(eflags);
    } else {
      this->obj->Error("Not implemented: rpl<cpl %s::Run",
                       this->code_name.c_str());
    }
    return;
  } else {
    this->obj->Error("Not implemented: 16bits mode at %s::Run",
                     this->code_name.c_str());
  }
  return;
}

MovEaxMoffs32::MovEaxMoffs32(string code_name) : Instruction(code_name) {}

void MovEaxMoffs32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32(EAX, memory.Read32(cpu.GetLinearAddrForDataAccess(
                        memory.Read32(cpu.GetLinearAddrForCodeAccess()))));
    cpu.AddEip(4);
    return;
  } else {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {  // 32bit_addr
      this->obj->Error("Not implemented: addr_size=32bit at %s::Run",
                       this->code_name.c_str());
    } else {  // 16bit_addr
      cpu.SetR16(EAX, memory.Read16(cpu.GetLinearAddrForDataAccess(
                          memory.Read16(cpu.GetLinearAddrForCodeAccess()))));
      cpu.AddEip(2);
    }
  }
  return;
}

MovMoffs32Eax::MovMoffs32Eax(string code_name) : Instruction(code_name) {}

void MovMoffs32Eax::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  // op_size = 32bit
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {  // 32bit_addr
      memory.Write(cpu.GetLinearAddrForDataAccess(
                       memory.Read32(cpu.GetLinearAddrForCodeAccess())),
                   cpu.GetR32(EAX));
      cpu.AddEip(4);
    } else {  // 16bit_addr
      memory.Write(cpu.GetLinearAddrForDataAccess(
                       memory.Read16(cpu.GetLinearAddrForCodeAccess())),
                   cpu.GetR32(EAX));
      cpu.AddEip(2);
    }
    return;
  }
  // op_size = 16bit
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {  // 32bit_addr
    memory.Write(cpu.GetLinearAddrForDataAccess(
                     memory.Read32(cpu.GetLinearAddrForCodeAccess())),
                 cpu.GetR16(EAX));
    cpu.AddEip(4);
  } else {  // 16bit_addr
    memory.Write(cpu.GetLinearAddrForDataAccess(
                     memory.Read16(cpu.GetLinearAddrForCodeAccess())),
                 cpu.GetR16(EAX));
    cpu.AddEip(2);
  }
}

SubR32Rm32::SubR32Rm32(string code_name) : Instruction(code_name) {}

void SubR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               cpu.Sub(r32, rm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             cpu.Sub(r16, rm16));
  return;
}

DecRm8::DecRm8(string code_name) : Instruction(code_name) {}

void DecRm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t r8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Dec(r8));
}

OrRm32Imm32::OrRm32Imm32(string code_name) : Instruction(code_name) {}

void OrRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    this->SetRM32(cpu, memory, cpu.Or(rm32, imm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  this->SetRM16(cpu, memory, cpu.Or(rm16, imm16));
  return;
}

NegRm32::NegRm32(string code_name) : Instruction(code_name) {}

void NegRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t result;
    uint32_t rm32;
    uint32_t zero = 0;
    rm32 = this->GetRM32(cpu, memory);
    if (rm32 == 0) {
      cpu.ClearFlag(CF);
    } else {
      cpu.SetFlag(CF);
    }
    result = (uint64_t)zero - (uint64_t)rm32;
    this->SetRM32(cpu, memory, zero - rm32);
    cpu.UpdateEflagsForDec(result, zero, rm32);
    return;
  }
  uint16_t result;
  uint16_t rm16;
  uint16_t zero = 0;
  rm16 = this->GetRM16(cpu, memory);
  if (rm16 == 0) {
    cpu.ClearFlag(CF);
  } else {
    cpu.SetFlag(CF);
  }
  result = zero - rm16;
  this->SetRM16(cpu, memory, result);
  cpu.UpdateEflagsForDec(result, zero, rm16);
  return;
}

TestEaxImm32::TestEaxImm32(string code_name) : Instruction(code_name) {}

void TestEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.And(cpu.GetR32(EAX), memory.Read32(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(4);
    return;
  }
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.And(ax, imm16);
  cpu.AddEip(2);
  return;
}

OrEaxImm32::OrEaxImm32(string code_name) : Instruction(code_name) {}

void OrEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eax = cpu.GetR32(EAX);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    cpu.SetR32(EAX, cpu.Or(eax, imm32));
    return;
  }
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  cpu.SetR16(EAX, cpu.Or(ax, imm16));
  return;
}

AddRm32Imm32::AddRm32Imm32(string code_name) : Instruction(code_name) {}

void AddRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    this->SetRM32(cpu, memory, cpu.Add(rm32, imm32));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  this->SetRM16(cpu, memory, cpu.Add(rm16, imm16));
  return;
}

XorRm32Imm8::XorRm32Imm8(string code_name) : Instruction(code_name) {}

void XorRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm8 =
        (int32_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(1);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Xor(imm8, rm32));
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SubEaxImm32::SubEaxImm32(string code_name) : Instruction(code_name) {}

//フラグレジスタの更新が未実装
void SubEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eax = cpu.GetR32(EAX);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    cpu.SetR32(EAX, cpu.Sub(eax, imm32));
    return;
  }
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  cpu.SetR16(EAX, cpu.Sub(ax, imm16));
  return;
}

ImulR32Rm32Imm8::ImulR32Rm32Imm8(string code_name) : Instruction(code_name) {}

void ImulR32Rm32Imm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm8 =
        (int32_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               this->GetRM32(cpu, memory) * imm8);
    cpu.AddEip(1);
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SetneRm8::SetneRm8(string code_name) : Instruction(code_name) {}

void SetneRm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if (!cpu.IsFlag(ZF)) {
      this->SetRM8(cpu, memory, 1);
    } else {
      this->SetRM8(cpu, memory, 0);
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SubRm8R8::SubRm8R8(string code_name) : Instruction(code_name) {}

void SubRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM8(cpu, memory, cpu.Sub(rm8, r8));
  return;
}

AdcRm8R8::AdcRm8R8(string code_name) : Instruction(code_name) {}

void AdcRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  this->SetRM8(cpu, memory, cpu.Adc(rm8, r8, cf));
  return;
}

Clc::Clc(string code_name) : Instruction(code_name) {}

void Clc::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.ClearFlag(CF);
}

AddR8Rm8::AddR8Rm8(string code_name) : Instruction(code_name) {}

void AddR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
            cpu.Add(rm8, r8));
}

SbbRm8R8::SbbRm8R8(string code_name) : Instruction(code_name) {}

void SbbRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM8(cpu, memory, cpu.Sub((uint8_t)rm8, (uint8_t)(r8 + cf)));
}

ShrRm32Cl::ShrRm32Cl(string code_name) : Instruction(code_name) {}

void ShrRm32Cl::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t cl = cpu.GetR8L(ECX);
    this->SetRM32(cpu, memory, cpu.Shr(rm32, cl));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t cl = cpu.GetR8L(ECX);
  this->SetRM16(cpu, memory, cpu.Shr(rm16, cl));
  return;
}

Xlatb::Xlatb(string code_name) : Instruction(code_name) {}

void Xlatb::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {  // xlatbはaddr_sizeで判別
    uint32_t al;
    al = (uint32_t)cpu.GetR8L(EAX);
    cpu.SetR8L(
        EAX,
        memory.Read8(cpu.GetLinearAddrForDataAccess(cpu.GetR32(EBX))) + al);
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

CmpRm8R8::CmpRm8R8(string code_name) : Instruction(code_name) {}

void CmpRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  cpu.Sub(rm8, r8);
}

CmpEaxImm32::CmpEaxImm32(string code_name) : Instruction(code_name) {}

void CmpEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    uint32_t eax = cpu.GetR32(EAX);
    cpu.Sub(eax, imm32);
    return;
  }
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  uint16_t ax = cpu.GetR16(EAX);
  cpu.Sub(ax, imm16);
  return;
}

TestRm32Imm32::TestRm32Imm32(string code_name) : Instruction(code_name) {}

void TestRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    cpu.And(rm32, imm32);
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  cpu.And(rm16, imm16);
  return;
}

LtrRm16::LtrRm16(string code_name) : Instruction(code_name) {}

void LtrRm16::Run(Cpu& cpu, Memory& memory) {
  cpu.SetTr(this->GetRM16(cpu, memory));
  return;
}

AndRm8R8::AndRm8R8(string code_name) : Instruction(code_name) {}

void AndRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM8(cpu, memory, cpu.And(rm8, r8));
  return;
}

JmpM1632::JmpM1632(string code_name) : Instruction(code_name) {}

// Farジャンプはプロテクトモードかリアルモードかで挙動が変わる。
void JmpM1632::Run(Cpu& cpu, Memory& memory) {
  if (cpu.IsRealMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      this->obj->Error(
          "Not implemented: op_size=32bit on real mode at %s::Run");
    }
    uint32_t addr;
    uint16_t offset_addr;
    uint16_t selector;
    addr = this->GetEffectiveAddr(cpu, memory);
    offset_addr = memory.Read16(cpu.GetLinearAddrForDataAccess(addr));
    selector = memory.Read16(cpu.GetLinearAddrForDataAccess(addr + 2));
    cpu.SetR16(CS, selector);
    cpu.SetEip((uint32_t)offset_addr);
    return;
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t addr;
    uint32_t offset_addr;
    uint16_t selector;
    addr = this->GetEffectiveAddr(cpu, memory);
    offset_addr = memory.Read32(cpu.GetLinearAddrForDataAccess(addr));
    selector = memory.Read16(cpu.GetLinearAddrForDataAccess(addr + 4));
    GdtGate* gdt_gate = cpu.GetGdtGate(selector);
    if ((gdt_gate->access_right & SEGMENT_DESC_TYPE_FLG) != 0) {
      cpu.SetR16(CS, selector);
      cpu.SetEip(offset_addr);
    } else {
      cpu.SaveTask(selector);
      cpu.SwitchTask();
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SeteRm8::SeteRm8(string code_name) : Instruction(code_name) {}

void SeteRm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if (cpu.IsFlag(ZF)) {
      this->SetRM8(cpu, memory, 1);
    } else {
      this->SetRM8(cpu, memory, 0);
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

MovAlMoffs8::MovAlMoffs8(string code_name) : Instruction(code_name) {}

void MovAlMoffs8::Run(Cpu& cpu, Memory& memory) {
  uint32_t offset32;
  uint8_t data;
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {  // 32bit_addr
    offset32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    data = memory.Read8(cpu.GetLinearAddrForDataAccess(offset32));
    cpu.SetR8L(EAX, data);
    return;
  }
  cpu.SetR8L(EAX, memory.Read8(cpu.GetLinearAddrForDataAccess(
                      memory.Read16(cpu.GetLinearAddrForCodeAccess()))));
  cpu.AddEip(2);
  return;
}

RcrRm8Imm8::RcrRm8Imm8(string code_name) : Instruction(code_name) {}

void RcrRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.Rcr(rm8, imm8));
  return;
}

SarRm8Imm8::SarRm8Imm8(string code_name) : Instruction(code_name) {}

void SarRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = (uint8_t)(memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.Sar(rm8, imm8));
  return;
}

Cld::Cld(string code_name) : Instruction(code_name) {}

void Cld::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.ClearFlag(DF);
  return;
}

CmpsM8M8::CmpsM8M8(string code_name) : Instruction(code_name) {}

void CmpsM8M8::Run(Cpu& cpu, Memory& memory) {
  // this->obj->Error("Not implemented: at %s::Run", this->code_name.c_str());
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment_override at %s::Run",
                     this->code_name.c_str());
  }
  cpu.AddEip(1);
  cpu.Cmps(1);  // TODO : マジックナンバーの修正
}

CmpsM32M32::CmpsM32M32(string code_name) : Instruction(code_name) {}

void CmpsM32M32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment_override at %s::Run",
                     this->code_name.c_str());
  }
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.Cmps(4);
  } else {
    cpu.Cmps(2);
  }
}

SetaRm8::SetaRm8(string code_name) : Instruction(code_name) {}

void SetaRm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if ((!cpu.IsFlag(ZF)) && (!cpu.IsFlag(CF))) {
      this->SetRM8(cpu, memory, 1);
    } else {
      this->SetRM8(cpu, memory, 0);
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SetbRm8::SetbRm8(string code_name) : Instruction(code_name) {}

void SetbRm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if (cpu.IsFlag(CF)) {
      this->SetRM8(cpu, memory, 1);
    } else {
      this->SetRM8(cpu, memory, 0);
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

TestRm8Imm8::TestRm8Imm8(string code_name) : Instruction(code_name) {}

void TestRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  cpu.And(rm8, imm8);
  return;
}

MovzxR32Rm16::MovzxR32Rm16(string code_name) : Instruction(code_name) {}

void MovzxR32Rm16::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint32_t rm16 = (uint32_t)this->GetRM16(cpu, memory);
  cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm16);
}

SbbR8Rm8::SbbR8Rm8(string code_name) : Instruction(code_name) {}

void SbbR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  cpu.SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
            cpu.Sub(r8, (uint8_t)(rm8 + cf)));
}

CallPtr1632::CallPtr1632(string code_name) : Instruction(code_name) {}

void CallPtr1632::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsRealMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      uint32_t offset;
      uint16_t selector;
      offset = memory.Read32(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(4);
      selector = memory.Read16(cpu.GetLinearAddrForCodeAccess());
      cpu.AddEip(2);
      cpu.Push32(cpu.GetR16(CS));
      cpu.Push32((uint16_t)cpu.GetEip());
      cpu.SetR16(CS, selector);
      cpu.SetEip(offset);
      return;
    }
    uint16_t offset;
    uint16_t selector;
    offset = memory.Read16(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(2);
    selector = memory.Read16(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(2);
    cpu.Push16(cpu.GetR16(CS));
    cpu.Push16((uint16_t)cpu.GetEip());
    cpu.SetR16(CS, selector);
    cpu.SetEip(offset);
    return;
  }
  GdtGate* gdt_gate;
  uint32_t imm32;
  uint16_t selector;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    selector = memory.Read16(cpu.GetLinearAddrForCodeAccess() + 4);
    gdt_gate = cpu.GetGdtGate(selector);
    cpu.AddEip(6);
    if ((gdt_gate->access_right & SEGMENT_DESC_TYPE_FLG) != 0) {
      cpu.Push32(cpu.GetR16(CS));
      cpu.Push32(cpu.GetEip());
      cpu.SetEip(imm32);
      cpu.SetR16(CS, selector);
      return;
    }
    /***
    #define TSS_TYPE 0x09
    #define LDT_TYPE 0x0082
    #define SEGMENT_DESC_TYPE_FLG 1<<4
    ***/
    if ((gdt_gate->access_right & CpuEnum::CALL_GATE) != 0) {
      uint8_t cpl = cpu.GetCpl();
      uint8_t call_gate_dpl = CpuHelper::GetDpl(gdt_gate->access_right);
      uint8_t call_gate_selector_rpl = CpuHelper::GetRpl(selector);
      uint8_t call_segment_selector = gdt_gate->base_low;
      uint8_t call_segment_dpl = CpuHelper::GetDpl(call_segment_selector);
      // GdtGate* call_segment_desc     = cpu.GetGdtGate(call_segment_selector);
      if ((call_gate_dpl < cpl) || (call_gate_dpl < call_gate_selector_rpl)) {
        this->obj->Error("Not implemented: #GP at %s::Run",
                         this->code_name.c_str());
      }
      if (call_segment_dpl < cpl) {  // MORE-PRIVILEGE
        uint16_t ss = cpu.GetR16(SS);
        uint32_t eip = cpu.GetEip();
        uint32_t esp = cpu.GetR32(ESP);
        Tss* tss = cpu.GetCurrentTss();
        cpu.SetR16(SS, tss->ss0);
        cpu.SetR32(ESP, tss->esp0);
        cpu.Push32(ss);
        cpu.Push32(esp);
        cpu.Push32(cpu.GetR16(CS));
        cpu.Push32(eip);
        CallGate* call_gate = (CallGate*)cpu.GetGdtGate(selector);
        uint32_t offset = (((uint32_t)call_gate->offset_high) << 16) |
                          ((uint32_t)call_gate->offset_low);
        cpu.SetEip(offset);
        cpu.SetR16(CS, call_segment_selector);
        return;
      } else {  // SAME-PRIVILEGE
        this->obj->Error("Not implemented: SAME-PRIVILEGE at %s::Run",
                         this->code_name.c_str());
      }
      this->obj->Error("Not implemented: CALL_GATE at %s::Run",
                       this->code_name.c_str());
    }
    this->obj->Error("Not implemented: TSS at %s::Run",
                     this->code_name.c_str());
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

Ret32Far::Ret32Far(string code_name) : Instruction(code_name) {}

void Ret32Far::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsRealMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      uint32_t eip;
      uint16_t cs;
      eip = cpu.Pop32();
      cs = cpu.Pop32();
      cpu.SetEip(eip);
      cpu.SetR16(CS, cs);
      return;
    }
    uint32_t eip;
    uint16_t cs;
    eip = cpu.Pop16();
    cs = cpu.Pop16();
    cpu.SetEip(eip);
    cpu.SetR16(CS, cs);
    return;
  }
  uint32_t eip;
  uint16_t cs;
  uint32_t esp;
  uint16_t ss;
  uint8_t cpl, rpl;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    eip = cpu.Pop32();
    cs = cpu.Pop32();
    rpl = CpuHelper::GetRpl(cs);
    cpl = cpu.GetCpl();
    if (rpl > cpl) {
      cpu.SetEip(eip);
      cpu.SetR16(CS, cs);
      esp = cpu.Pop32();
      ss = cpu.Pop32();
      cpu.SetR32(ESP, esp);
      cpu.SetR16(SS, ss);
    } else {
      cpu.SetEip(eip);
      cpu.SetR16(CS, cs);
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

CallM1632::CallM1632(string code_name) : Instruction(code_name) {}

void CallM1632::Run(Cpu& cpu, Memory& memory) {
  GdtGate* gdt_gate;
  uint16_t selector;
  if (cpu.IsProtectedMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      uint32_t effective_addr;
      uint32_t offset_addr;
      effective_addr = this->GetEffectiveAddr(cpu, memory);
      offset_addr =
          memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr));
      selector =
          memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 4));
      gdt_gate = cpu.GetGdtGate(selector);
      if ((gdt_gate->access_right & SEGMENT_DESC_TYPE_FLG) != 0) {
        cpu.Push32(cpu.GetR16(CS));
        cpu.Push32(cpu.GetEip());
        cpu.SetEip(offset_addr);
        cpu.SetR16(CS, selector);
        return;
      }
      this->obj->Error("Not implemented: TSS at %s::Run",
                       this->code_name.c_str());
      return;
    }
    this->obj->Error("Not implemented: 16bits mode at %s::Run",
                     this->code_name.c_str());
    return;
  }
  //リアルモードにもcall命令で32bit op sizeはある。
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint16_t effective_addr;
    uint32_t eip;
    effective_addr = this->GetEffectiveAddr(cpu, memory);
    eip = memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr));
    selector =
        memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 4));
    cpu.Push32(cpu.GetR16(CS));
    cpu.Push32((uint16_t)cpu.GetEip());
    cpu.SetEip(eip);
    cpu.SetR16(CS, selector);
    return;
  }
  uint16_t effective_addr;
  uint16_t eip;
  effective_addr = this->GetEffectiveAddr(cpu, memory);
  eip = memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr));
  selector = memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 2));
  cpu.Push16(cpu.GetR16(CS));
  cpu.Push16((uint16_t)cpu.GetEip());
  cpu.SetEip(eip);
  cpu.SetR16(CS, selector);
}

CmpR8Rm8::CmpR8Rm8(string code_name) : Instruction(code_name) {}

void CmpR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  cpu.Sub(r8, rm8);
  return;
}

MulRm32::MulRm32(string code_name) : Instruction(code_name) {}

void MulRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint64_t rm32;
    uint64_t eax;
    uint64_t result;
    eax = cpu.GetR32(EAX);
    rm32 = this->GetRM32(cpu, memory);
    result = eax * rm32;
    cpu.SetR32(EAX, result & 0x00000000FFFFFFFF);
    cpu.SetR32(EDX, (result & 0xFFFFFFFF00000000) >> 32);
    if (!cpu.GetR32(EDX)) {
      cpu.ClearFlag(OF);
      cpu.ClearFlag(CF);
    } else {
      cpu.SetFlag(OF);
      cpu.SetFlag(CF);
    }
    return;
  }
  uint32_t rm16;
  uint32_t ax;
  uint32_t result;
  ax = cpu.GetR16(EAX);
  rm16 = this->GetRM16(cpu, memory);
  result = ax * rm16;
  cpu.SetR16(EAX, result & 0x0000FFFF);
  cpu.SetR16(EDX, (result & 0xFFFF0000) >> 16);
  if (!cpu.GetR16(EDX)) {
    cpu.ClearFlag(OF);
    cpu.ClearFlag(CF);
  } else {
    cpu.SetFlag(OF);
    cpu.SetFlag(CF);
  }
  return;
}

AddRm8R8::AddRm8R8(string code_name) : Instruction(code_name) {}

void AddRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8(this->modrm.reg_index);
  this->SetRM8(cpu, memory, cpu.Add(rm8, r8));
  return;
}

JmpRm32::JmpRm32(string code_name) : Instruction(code_name) {}

void JmpRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.SetEip(rm32);
    return;
  }
  uint32_t rm16 = this->GetRM16(cpu, memory);
  cpu.SetEip(rm16);
  return;
}

ShrRm32::ShrRm32(string code_name) : Instruction(code_name) {}

void ShrRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Shr(rm32, (uint32_t)1));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Shr(rm16, (uint16_t)1));
  return;
}

SalRm32Cl::SalRm32Cl(string code_name) : Instruction(code_name) {}

void SalRm32Cl::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t cl = cpu.GetR8L(ECX);
    this->SetRM32(cpu, memory, cpu.Sal(rm32, cl));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t cl = cpu.GetR8L(ECX);
  this->SetRM16(cpu, memory, cpu.Sal(rm16, cl));
}

SetgRm8::SetgRm8(string code_name) : Instruction(code_name) {}

void SetgRm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    if ((!cpu.IsFlag(ZF)) && (cpu.IsFlag(CF) == cpu.IsFlag(OF))) {
      this->SetRM8(cpu, memory, 1);
    } else {
      this->SetRM8(cpu, memory, 0);
    }
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SarRm32Cl::SarRm32Cl(string code_name) : Instruction(code_name) {}

void SarRm32Cl::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    uint32_t cl = cpu.GetR8L(ECX);
    this->SetRM32(cpu, memory, cpu.Sar(rm32, cl));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t cl = cpu.GetR8L(ECX);
  this->SetRM16(cpu, memory, cpu.Sar(rm16, cl));
  return;
}

AndRm32R32::AndRm32R32(string code_name) : Instruction(code_name) {}

void AndRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.And(rm32, r32));
    return;
  }
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.And(rm16, r16));
  return;
}

SalRm32::SalRm32(string code_name) : Instruction(code_name) {}

void SalRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Sal(rm32, (uint32_t)1));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Sal(rm16, (uint16_t)1));
  return;
}

AndR32Rm32::AndR32Rm32(string code_name) : Instruction(code_name) {}

void AndR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t r32 =
        cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               cpu.And(rm32, r32));
    return;
  }
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             cpu.And(rm16, r16));
  return;
}

ShrdRm32R32Imm8::ShrdRm32R32Imm8(string code_name) : Instruction(code_name) {}

void ShrdRm32R32Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t imm8;
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  if (imm8 == 8) {
    this->obj->Error("Not implemented: imm8==1 at %s::Run",
                     this->code_name.c_str());
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint64_t rm32, r32;
    uint64_t dest;
    rm32 = this->GetRM32(cpu, memory);
    r32 = cpu.GetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
    dest = (r32 << 32) | rm32;
    dest = dest >> (imm8 - 1);
    if (dest & 0x01) {
      cpu.SetFlag(CF);
    } else {
      cpu.ClearFlag(CF);
    }
    dest = dest >> 1;
    rm32 = (uint32_t)dest;
    this->SetRM32(cpu, memory, rm32);
    cpu.UpdateEflagsForShr(rm32);
    return;
  }
  this->obj->Error("Not implemented: 16bits mode at %s::Run",
                   this->code_name.c_str());
}

SarRm32::SarRm32(string code_name) : Instruction(code_name) {}

void SarRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    this->SetRM32(cpu, memory, cpu.Sar(rm32, (uint32_t)1));
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Sar(rm16, (uint16_t)1));
  return;
}

SetgeRm8::SetgeRm8(string code_name) : Instruction(code_name) {}

void SetgeRm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.IsFlag(SF) == cpu.IsFlag(OF)) {
    this->SetRM8(cpu, memory, 1);
  } else {
    this->SetRM8(cpu, memory, 0);
  }
  return;
}

CallRm32::CallRm32(string code_name) : Instruction(code_name) {}

void CallRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rm32 = this->GetRM32(cpu, memory);
    cpu.Push32(cpu.GetEip());
    cpu.SetEip(rm32);
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  cpu.Push16(cpu.GetEip());
  cpu.SetEip(rm16);
  return;
}

MovM32M32::MovM32M32(string code_name) : Instruction(code_name) {}

void MovM32M32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint32_t ds, es;
  uint32_t esi, edi;
  uint32_t d;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    d = cpu.IsFlag(DF) ? -4 : 4;
  } else {
    d = cpu.IsFlag(DF) ? -2 : 2;
  }
  ds = cpu.GetBaseAddr(DS);
  es = cpu.GetBaseAddr(ES);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    esi = cpu.GetR32(ESI);
    edi = cpu.GetR32(EDI);
  } else {
    esi = cpu.GetR16(ESI);
    edi = cpu.GetR16(EDI);
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    memory.Write(es + edi, memory.Read32(ds + esi));
  } else {
    memory.Write(es + edi, memory.Read16(ds + esi));
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    cpu.SetR32(EDI, edi + d);
    cpu.SetR32(ESI, esi + d);
  } else {
    cpu.SetR16(EDI, edi + d);
    cpu.SetR16(ESI, esi + d);
  }
  return;
}

AdcRm32Imm8::AdcRm32Imm8(string code_name) : Instruction(code_name) {}

void AdcRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: 32bits mode at %s::Run",
                     this->code_name.c_str());
    return;
  }
  uint16_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm8 =
      (int16_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  this->SetRM16(cpu, memory, cpu.Adc(rm16, imm8, cf));
  cpu.AddEip(1);
  return;
}

Cwde::Cwde(string code_name) : Instruction(code_name) {}

void Cwde::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: 32bits mode at %s::Run",
                     this->code_name.c_str());
    return;
  }
  uint16_t al = (int16_t)(int8_t)cpu.GetR8L(EAX);
  cpu.SetR16(EAX, al);
}

AdcRm32R32::AdcRm32R32(string code_name) : Instruction(code_name) {}

void AdcRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: 32bits mode at %s::Run",
                     this->code_name.c_str());
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint16_t cf = cpu.IsFlag(CF) ? 1 : 0;
  this->SetRM16(cpu, memory, cpu.Adc(rm16, r16, cf));
}

LodsM8::LodsM8(string code_name) : Instruction(code_name) {}

void LodsM8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    uint32_t ds;
    uint32_t esi;
    uint32_t d = cpu.IsFlag(DF) ? -1 : 1;
    ;
    ds = cpu.GetBaseAddr(DS);
    esi = cpu.GetR32(ESI);
    cpu.SetR8L(EAX, memory.Read8(cpu.GetPhysicalAddr(ds + esi)));
    cpu.SetR32(ESI, esi + d);
    return;
  }
  uint32_t ds;
  uint16_t si;
  uint16_t d;
  ds = cpu.GetBaseAddr(DS);
  si = cpu.GetR16(ESI);
  cpu.SetR8L(EAX, memory.Read8(cpu.GetPhysicalAddr(ds + si)));
  d = cpu.IsFlag(DF) ? -1 : 1;
  cpu.SetR16(ESI, si + d);
  return;
}

LesR32M1632::LesR32M1632(string code_name) : Instruction(code_name) {}

void LesR32M1632::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr)));
    cpu.SetR16(
        ES, memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 4)));
    return;
  }
  uint32_t effective_addr;
  effective_addr = this->GetEffectiveAddr(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr)));
  // fprintf(stderr, "ES=%04X\n",
  // memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr+2)));
  cpu.SetR16(ES,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 2)));
}

MulRm8::MulRm8(string code_name) : Instruction(code_name) {}

void MulRm8::Run(Cpu& cpu, Memory& memory) {
  uint16_t rm8 = this->GetRM8(cpu, memory);
  uint16_t al = cpu.GetR8L(EAX);
  cpu.SetR16(EAX, rm8 * al);
  if (!cpu.GetR8H(EAX)) {
    cpu.ClearFlag(OF);
    cpu.ClearFlag(CF);
  } else {
    cpu.SetFlag(OF);
    cpu.SetFlag(CF);
  }
}

XchgEaxR32::XchgEaxR32(string code_name) : Instruction(code_name) {}

void XchgEaxR32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implmented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  GENERAL_PURPOSE_REGISTER32 register_type =
      (GENERAL_PURPOSE_REGISTER32)((uint32_t)memory.Read8(
                                       cpu.GetLinearAddrForCodeAccess()) -
                                   (uint32_t)0x90);
  cpu.AddEip(1);
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t r16 = cpu.GetR16(register_type);
  cpu.SetR16(EAX, r16);
  cpu.SetR16(register_type, ax);
}

DivRm8::DivRm8(string code_name) : Instruction(code_name) {}

void DivRm8::Run(Cpu& cpu, Memory& memory) {
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t rm8 = this->GetRM8(cpu, memory);
  cpu.SetR8L(EAX, ax / rm8);
  cpu.SetR8H(EAX, ax % rm8);
}

XchgRm8R8::XchgRm8R8(string code_name) : Instruction(code_name) {}

void XchgRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8, r8;
  rm8 = this->GetRM8(cpu, memory);
  r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM8(cpu, memory, r8);
  cpu.SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index, rm8);
  return;
}

RorRm8::RorRm8(string code_name) : Instruction(code_name) {}

void RorRm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Ror(rm8, (uint8_t)1));
  return;
}

IncRm8::IncRm8(string code_name) : Instruction(code_name) {}

void IncRm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Inc(rm8));
  return;
}

OrRm8R8::OrRm8R8(string code_name) : Instruction(code_name) {}

void OrRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM8(cpu, memory, cpu.Or(rm8, r8));
  return;
}

StosM32::StosM32(string code_name) : Instruction(code_name) {}

void StosM32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.Stos(cpu.GetR32(EAX));
    return;
  }
  cpu.Stos(cpu.GetR16(EAX));
  return;
}

LodsM32::LodsM32(string code_name) : Instruction(code_name) {}

void LodsM32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  uint32_t ds = cpu.GetBaseAddr(DS);
  uint32_t esi;
  uint32_t d;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    d = cpu.IsFlag(DF) ? -4 : 4;
  } else {
    d = cpu.IsFlag(DF) ? -2 : 2;
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    esi = cpu.GetR32(ESI);
  } else {
    esi = cpu.GetR16(ESI);
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    cpu.SetR32(EAX, memory.Read32(cpu.GetPhysicalAddr(ds + esi)));
  } else {
    cpu.SetR16(EAX, memory.Read16(cpu.GetPhysicalAddr(ds + esi)));
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    cpu.SetR32(ESI, esi + d);
  } else {
    cpu.SetR16(ESI, esi + d);
  }

  return;
}

AndRm8Imm8::AndRm8Imm8(string code_name) : Instruction(code_name) {}

void AndRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.And(rm8, imm8));
  return;
}

AdcR32Rm32::AdcR32Rm32(string code_name) : Instruction(code_name) {}

void AdcR32Rm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run");
  }
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint16_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Adc(r16, rm16, cf));
  return;
}

Code66::Code66(string code_name) : Instruction(code_name) {}

void Code66::Run(Cpu& cpu, Memory& memory) {
  cpu.SetPrefix66();
  cpu.AddEip(1);
  return;
}

CodeF3::CodeF3(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0x66] = make_unique<Code66>("Code66");
  this->instructions[0xA4] = make_unique<RepMovsM8M8>("RepMovsM8M8");
  this->instructions[0xA5] = make_unique<RepMovsM32M32>("RepMovsM32M32");
  this->instructions[0xA6] = make_unique<RepeCmpsM8M8>("RepeCmpsM8M8");
  this->instructions[0xA7] = make_unique<RepeCmpsM32M32>("RepeCmpsM32M32");
  this->instructions[0xAA] = make_unique<RepStosM8>("RepStosM8");
  this->instructions[0xAB] = make_unique<RepStosM32>("RepStosM32");
  this->instructions[0xAE] = make_unique<RepeScasM8>("RepeScasM8");
  this->instructions[0xAF] = make_unique<RepeScasM32>("RepeScasM32");
}

void CodeF3::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t op_code = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  if (op_code == 0x66) {
    this->instructions[0x66]->Run(cpu, memory);
    op_code = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  }
  if (this->instructions[op_code].get() == NULL) {
    this->obj->Error("Not implemented: F3 %02X at %s::Run", op_code,
                     this->code_name.c_str());
  }
  this->instructions[op_code]->Run(cpu, memory);
  return;
}

RepeCmpsM8M8::RepeCmpsM8M8(string code_name) : Instruction(code_name) {}

void RepeCmpsM8M8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.Rep([&]() { cpu.Cmps(1); }, [&]() { return !cpu.IsFlag(ZF); });
  return;
}

CodeF2::CodeF2(string code_name) : Instruction(code_name) {
  for (int i = 0; i < InstructionHelper::INSTRUCTION_SIZE; i++) {
    this->instructions[i] = NULL;
  }
  this->instructions[0xAE] = make_unique<RepneScasM8>("RepneScasM8");
}

void CodeF2::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t op_code = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  if (this->instructions[op_code].get() == NULL) {
    this->obj->Error("Not implemented: F2 %02X at %s::Run", op_code,
                     this->code_name.c_str());
  }
  this->instructions[op_code]->Run(cpu, memory);
  return;
}

RepneScasM8::RepneScasM8(string code_name) : Instruction(code_name) {}

void RepneScasM8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  cpu.Rep([&]() { cpu.Scas(al); }, [&]() { return cpu.IsFlag(ZF); });
  return;
}

AddAlImm8::AddAlImm8(string code_name) : Instruction(code_name) {}

void AddAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  cpu.SetR8L(EAX, cpu.Add(al, imm8));
  return;
}

OrR8Rm8::OrR8Rm8(string code_name) : Instruction(code_name) {}

void OrR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR8(this->modrm.reg_index, cpu.Or(rm8, r8));
  return;
}

OrR32Rm32::OrR32Rm32(string code_name) : Instruction(code_name) {}

void OrR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             cpu.Or(rm16, r16));
  return;
}

OrAlImm8::OrAlImm8(string code_name) : Instruction(code_name) {}

void OrAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  cpu.SetR8L(EAX, cpu.Or(al, imm8));
  return;
}

SbbRm32R32::SbbRm32R32(string code_name) : Instruction(code_name) {}

void SbbRm32R32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM16(cpu, memory,
                cpu.Sub(rm16, (uint16_t)((uint16_t)r16 + (uint16_t)cf)));
  return;
}

SbbR32Rm32::SbbR32Rm32(string code_name) : Instruction(code_name) {}

void SbbR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             cpu.Sub(r16, (uint16_t)((uint16_t)rm16 + (uint16_t)cf)));
  return;
}

SubR8Rm8::SubR8Rm8(string code_name) : Instruction(code_name) {}

void SubR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
            cpu.Sub(r8, rm8));
}

SubAlImm8::SubAlImm8(string code_name) : Instruction(code_name) {}

void SubAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  cpu.SetR8L(EAX, cpu.Sub(al, imm8));
  return;
}

XorRm8R8::XorRm8R8(string code_name) : Instruction(code_name) {}

void XorRm8R8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  this->SetRM8(cpu, memory, cpu.Xor(rm8, r8));
  return;
}

XorR8Rm8::XorR8Rm8(string code_name) : Instruction(code_name) {}

void XorR8Rm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t r8 = cpu.GetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR8((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
            cpu.Xor(rm8, r8));
  return;
}

XorR32Rm32::XorR32Rm32(string code_name) : Instruction(code_name) {}

void XorR32Rm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
    return;
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t r16 = cpu.GetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             cpu.Xor(rm16, r16));
  return;
}

OrRm8Imm8::OrRm8Imm8(string code_name) : Instruction(code_name) {}

void OrRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.Or(rm8, imm8));
  return;
}

SubRm8Imm8::SubRm8Imm8(string code_name) : Instruction(code_name) {}

void SubRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t rm8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Sub(rm8, imm8));
  return;
}

XorRm8Imm8::XorRm8Imm8(string code_name) : Instruction(code_name) {}

void XorRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.Xor(rm8, imm8));
  return;
}

XorRm32Imm32::XorRm32Imm32(string code_name) : Instruction(code_name) {}

void XorRm32Imm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: 32bit=op_size at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  this->SetRM16(cpu, memory, cpu.Xor(rm16, imm16));
  return;
}

SbbRm32Imm8::SbbRm32Imm8(string code_name) : Instruction(code_name) {}

void SbbRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint16_t rm16 = this->GetRM16(cpu, memory);
  uint16_t imm8 =
      (int16_t)(int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM16(cpu, memory,
                cpu.Sub(rm16, (uint16_t)((uint16_t)imm8 + (uint16_t)cf)));
  return;
}

PopM32::PopM32(string code_name) : Instruction(code_name) {}

void PopM32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t effective_addr;
    effective_addr = this->GetEffectiveAddr(cpu, memory);
    memory.Write(cpu.GetLinearAddrForDataAccess(effective_addr), cpu.Pop32());
    return;
  }
  uint32_t effective_addr;
  effective_addr = this->GetEffectiveAddr(cpu, memory);
  memory.Write(cpu.GetLinearAddrForDataAccess(effective_addr), cpu.Pop16());
  return;
}

Lahf::Lahf(string code_name) : Instruction(code_name) {}

void Lahf::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t eflags = cpu.GetEflgs();
  eflags = eflags | 0x02;
  cpu.SetR8H(EAX, eflags);
}

MovsM8M8::MovsM8M8(string code_name) : Instruction(code_name) {}

void MovsM8M8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    uint32_t d = cpu.IsFlag(DF) ? -1 : 1;
    uint32_t ds = cpu.GetBaseAddr(DS);
    uint32_t esi = cpu.GetR32(ESI);
    uint32_t es = cpu.GetBaseAddr(ES);
    uint32_t edi = cpu.GetR32(EDI);
    memory.Write(cpu.GetPhysicalAddr(es + edi),
                 memory.Read8(cpu.GetPhysicalAddr(ds + esi)));
    cpu.SetR32(EDI, edi + d);
    cpu.SetR32(ESI, esi + d);
    return;
  }
  uint32_t ds = cpu.GetBaseAddr(DS);
  uint16_t si = cpu.GetR16(ESI);
  uint32_t es = cpu.GetBaseAddr(ES);
  uint16_t di = cpu.GetR16(EDI);
  memory.Write(cpu.GetPhysicalAddr(es + di),
               memory.Read8(cpu.GetPhysicalAddr(ds + si)));
  uint16_t d = cpu.IsFlag(DF) ? -1 : 1;
  cpu.SetR16(EDI, di + d);
  cpu.SetR16(ESI, si + d);
  return;
}

TestAlImm8::TestAlImm8(string code_name) : Instruction(code_name) {}

void TestAlImm8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  cpu.And(al, imm8);
  return;
}

StosM8::StosM8(string code_name) : Instruction(code_name) {}

void StosM8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  cpu.Stos(cpu.GetR8L(EAX));
  return;
}

ScasM8::ScasM8(string code_name) : Instruction(code_name) {}

void ScasM8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  uint8_t al = cpu.GetR8L(EAX);
  cpu.Scas(al);
  return;
}

ScasD::ScasD(string code_name) : Instruction(code_name) {}

void ScasD::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eax = cpu.GetR32(EAX);
    cpu.Scas(eax);
  } else {
    uint16_t ax = cpu.GetR16(EAX);
    cpu.Scas(ax);
  }
  return;
}

//左回転
RclRm32Imm8::RclRm32Imm8(string code_name) : Instruction(code_name) {}

void RclRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t imm8 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Rcl(rm16, imm8));
}

RetImm16::RetImm16(string code_name) : Instruction(code_name) {}

void RetImm16::Run(Cpu& cpu, Memory& memory) {
  if (cpu.IsProtectedMode()) {  //下はリアルモード仕様なので、ストップ
    this->obj->Error("Not implemented: protected mode at %s::Run",
                     this->code_name.c_str());
  }
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint32_t addr;
  addr = cpu.Pop16();
  uint16_t imm16 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  cpu.SetEip(addr);
  for (uint16_t i = 0; i < imm16; i++) {
    cpu.Pop8();  //指定された回数だけPop
  }
  return;
}

LdsR32M1632::LdsR32M1632(string code_name) : Instruction(code_name) {}

void LdsR32M1632::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.IsProtectedMode()) {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
      cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
                 memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr)));
      cpu.SetR16(DS, memory.Read16(
                         cpu.GetLinearAddrForDataAccess(effective_addr + 4)));
      return;
    }
    this->obj->Error("Not implemented: op_size=16bit_size at %s::Run",
                     this->code_name.c_str());
    return;
  } else {
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
      cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
                 memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr)));
      cpu.SetR16(DS, memory.Read16(
                         cpu.GetLinearAddrForDataAccess(effective_addr + 4)));
      return;
    }
    uint16_t effective_addr;
    effective_addr = this->GetEffectiveAddr(cpu, memory);
    cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr)));
    // fprintf(stderr, "ES=%04X\n",
    // memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr+2)));
    cpu.SetR16(
        DS, memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 2)));
  }
}

RetFarImm16::RetFarImm16(string code_name) : Instruction(code_name) {}

void RetFarImm16::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsProtectedMode()) {
    this->obj->Error("Not implemented: protected mode at %s::Run",
                     this->code_name.c_str());
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t ip, cs;
  ip = cpu.Pop16();
  cs = cpu.Pop16();
  uint16_t imm16 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  cpu.SetEip(ip);
  cpu.SetR16(CS, cs);
  for (uint16_t i = 0; i < imm16; i++) {
    cpu.Pop8();  //指定された回数だけPop
  }
  return;
}

//左回転
RclRm32::RclRm32(string code_name) : Instruction(code_name) {}

void RclRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Rcl(rm16, (uint16_t)1));
}

SalRm8Cl::SalRm8Cl(string code_name) : Instruction(code_name) {}

void SalRm8Cl::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t cl = cpu.GetR8L(ECX);
  this->SetRM8(cpu, memory, cpu.Sal(rm8, cl));
}

LoopeRel8::LoopeRel8(string code_name) : Instruction(code_name) {}

void LoopeRel8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint32_t count;
  //アドレスサイズによって、カウンタの値が決まる。
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    count = cpu.GetR32(ECX);
    count--;
    cpu.SetR32(ECX, count);
  } else {
    count = cpu.GetR16(ECX);
    count--;
    cpu.SetR16(ECX, count);
  }
  uint32_t rel8 =
      (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  if (count && cpu.IsFlag(ZF)) {
    cpu.AddEip(rel8);
  }
  return;
}

LoopRel8::LoopRel8(string code_name) : Instruction(code_name) {}

void LoopRel8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint32_t count;
  //アドレスサイズによって、カウンタの値が決まる。
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    count = cpu.GetR32(ECX);
    count--;
    cpu.SetR32(ECX, count);
  } else {
    count = cpu.GetR16(ECX);
    count--;
    cpu.SetR16(ECX, count);
  }
  uint32_t rel8 =
      (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  if (count) {
    cpu.AddEip(rel8);
  }
  return;
}

JcxzRel8::JcxzRel8(string code_name) : Instruction(code_name) {}

void JcxzRel8::Run(Cpu& cpu, Memory& memory) {
  //アドレスサイズが32bitの時は、ECXを使用するので、未実装
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    uint32_t rel8;
    cpu.AddEip(1);
    rel8 = (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
    cpu.AddEip(1);
    if (!cpu.GetR32(ECX)) {
      cpu.AddEip(rel8);
    }
    return;
  }
  uint32_t rel8;
  cpu.AddEip(1);
  rel8 = (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  if (!cpu.GetR16(ECX)) {
    cpu.AddEip(rel8);
  }
  return;
}

RepMovsM8M8::RepMovsM8M8(string code_name) : Instruction(code_name) {}

void RepMovsM8M8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {  // 32bit op_size
    uint32_t cnt;
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
      cnt = cpu.GetR32(ECX);
    } else {
      cnt = cpu.GetR16(ECX);
    }
    uint32_t ds, es;
    ds = cpu.GetBaseAddr(DS);
    es = cpu.GetBaseAddr(ES);
    uint32_t d = cpu.IsFlag(DF) ? -1 : 1;
    uint16_t si, di;
    uint32_t esi, edi;
    for (uint16_t i = 0; i < cnt; i++) {
      if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
        esi = cpu.GetR32(ESI);
        edi = cpu.GetR32(EDI);
      } else {
        si = cpu.GetR16(ESI);
        di = cpu.GetR16(EDI);
      }
      if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
        memory.Write(cpu.GetPhysicalAddr(es + edi),
                     memory.Read8(cpu.GetPhysicalAddr(ds + esi)));
      } else {
        memory.Write(cpu.GetPhysicalAddr(es + di),
                     memory.Read8(cpu.GetPhysicalAddr(ds + si)));
      }
      if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
        cpu.SetR32(EDI, edi + d);
        cpu.SetR32(ESI, esi + d);
        cpu.SetR32(ECX, cpu.GetR32(ECX) - 1);
      } else {
        cpu.SetR16(EDI, di + d);
        cpu.SetR16(ESI, si + d);
        cpu.SetR16(ECX, cpu.GetR16(ECX) - 1);
      }
    }
    return;
  } else {  // 16bit op_size
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
      this->obj->Error(
          "Not implemented: addr_size=32bits && addr_size=32bits at %s::Run",
          this->code_name.c_str());
    }
    uint16_t cx = cpu.GetR16(ECX);
    uint32_t ds, es;
    ds = cpu.GetBaseAddr(DS);
    es = cpu.GetBaseAddr(ES);
    for (uint16_t i = 0; i < cx; i++) {
      uint16_t si, di;
      uint16_t d;
      si = cpu.GetR16(ESI);
      di = cpu.GetR16(EDI);
      memory.Write(es + di, memory.Read8(ds + si));
      d = cpu.IsFlag(DF) ? -1 : 1;
      cpu.SetR16(EDI, di + d);
      cpu.SetR16(ESI, si + d);
      cpu.SetR16(ECX, cpu.GetR16(ECX) - 1);
    }
  }
  return;
}

RepMovsM32M32::RepMovsM32M32(string code_name) : Instruction(code_name) {}

void RepMovsM32M32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.IsSegmentOverride()) {
    this->obj->Error("Not implemented: segment override at %s::Run",
                     this->code_name.c_str());
  }
  uint32_t cnt;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    cnt = cpu.GetR32(ECX);
  } else {
    cnt = cpu.GetR16(ECX);
  }
  uint32_t ds = cpu.GetBaseAddr(DS);
  uint32_t es = cpu.GetBaseAddr(ES);
  uint32_t d;
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    d = cpu.IsFlag(DF) ? -4 : 4;
  } else {
    d = cpu.IsFlag(DF) ? -2 : 2;
  }
  for (uint32_t i = 0; i < cnt; i++) {
    uint32_t esi, edi;
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
      esi = cpu.GetR32(ESI);
      edi = cpu.GetR32(EDI);
    } else {
      esi = cpu.GetR16(ESI);
      edi = cpu.GetR16(EDI);
    }
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
      memory.Write(cpu.GetPhysicalAddr(es + edi),
                   memory.Read32(cpu.GetPhysicalAddr(ds + esi)));
    } else {
      memory.Write(cpu.GetPhysicalAddr(es + edi),
                   memory.Read16(cpu.GetPhysicalAddr(ds + esi)));
    }
    if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
      cpu.SetR32(EDI, edi + d);
      cpu.SetR32(ESI, esi + d);
      cpu.SetR32(ECX, cpu.GetR32(ECX) - 1);
    } else {
      cpu.SetR16(EDI, edi + d);
      cpu.SetR16(ESI, esi + d);
      cpu.SetR16(ECX, cpu.GetR16(ECX) - 1);
    }
  }
  return;
}

RepStosM8::RepStosM8(string code_name) : Instruction(code_name) {}

void RepStosM8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  cpu.Rep([&]() { cpu.Stos(al); }, [&]() { return false; });
}

RepStosM32::RepStosM32(string code_name) : Instruction(code_name) {}

void RepStosM32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {  // 32bit op_size
    uint32_t eax = cpu.GetR32(EAX);
    cpu.Rep([&]() { cpu.Stos(eax); }, [&]() { return false; });
    return;
  }
  uint16_t ax = cpu.GetR16(EAX);
  cpu.Rep([&]() { cpu.Stos(ax); }, [&]() { return false; });
  return;
}

NotRm8::NotRm8(string code_name) : Instruction(code_name) {}

void NotRm8::Run(Cpu& cpu, Memory& memory) {
  uint32_t rm32;
  rm32 = this->GetRM8(cpu, memory);
  rm32 = ~rm32;
  this->SetRM8(cpu, memory, rm32);
  return;
}

ImulRm16::ImulRm16(string code_name) : Instruction(code_name) {}

void ImulRm16::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {  // 32bit_op
    uint64_t rm32;
    uint64_t eax;
    uint64_t result;
    eax = (int64_t)(int32_t)cpu.GetR32(EAX);
    rm32 = (int64_t)(int32_t)this->GetRM32(cpu, memory);
    result = eax * rm32;
    cpu.SetR32(EAX, result & 0x00000000FFFFFFFF);
    cpu.SetR32(EDX, (result & 0xFFFFFFFF00000000) >> 32);
    if (!cpu.GetR32(EDX)) {
      cpu.ClearFlag(OF);
      cpu.ClearFlag(CF);
    } else {
      cpu.SetFlag(OF);
      cpu.SetFlag(CF);
    }
    return;
  }
  uint32_t rm16;
  uint32_t ax;
  uint32_t result;
  ax = (int32_t)(int16_t)cpu.GetR16(EAX);
  rm16 = (int32_t)(int16_t)this->GetRM16(cpu, memory);
  result = ax * rm16;
  cpu.SetR16(EAX, result & 0x0000FFFF);
  cpu.SetR16(EDX, (result & 0xFFFF0000) >> 16);
  if (!cpu.GetR16(EDX)) {
    cpu.ClearFlag(OF);
    cpu.ClearFlag(CF);
  } else {
    cpu.SetFlag(OF);
    cpu.SetFlag(CF);
  }
  return;
}

Stc::Stc(string code_name) : Instruction(code_name) {}

void Stc::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.SetFlag(CF);
}

Std::Std(string code_name) : Instruction(code_name) {}

void Std::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.SetFlag(DF);
  return;
}

ShrRm8::ShrRm8(string code_name) : Instruction(code_name) {}

void ShrRm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  this->SetRM8(cpu, memory, cpu.Shr(rm8, (uint8_t)1));
}

AdcRm8Imm8::AdcRm8Imm8(string code_name) : Instruction(code_name) {}

void AdcRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  uint8_t cf = cpu.IsFlag(CF) ? 1 : 0;
  this->SetRM8(cpu, memory, cpu.Adc(rm8, imm8, cf));
  return;
}

XorEaxImm32::XorEaxImm32(string code_name) : Instruction(code_name) {}

void XorEaxImm32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t eax = cpu.GetR32(EAX);
    uint32_t imm32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    cpu.SetR32(EAX, cpu.Xor(eax, imm32));
    return;
  }
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(2);
  cpu.SetR16(EAX, cpu.Xor(ax, imm16));
  return;
}

RepeCmpsM32M32::RepeCmpsM32M32(string code_name) : Instruction(code_name) {}

void RepeCmpsM32M32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.Rep(
      [&]() {
        if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
          cpu.Cmps(4);
        } else {
          cpu.Cmps(2);
        }
      },
      [&]() { return !cpu.IsFlag(ZF); });
  return;
}

RepeScasM8::RepeScasM8(string code_name) : Instruction(code_name) {}

void RepeScasM8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint8_t al = cpu.GetR8L(EAX);
  cpu.Rep([&]() { cpu.Scas(al); }, [&]() { return !cpu.IsFlag(ZF); });
  return;
}

RcrRm32::RcrRm32(string code_name) : Instruction(code_name) {}

void RcrRm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {  // 32bit op_size
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Rcr(rm16, (uint16_t)1));
  return;
}

AdcEaxImm32::AdcEaxImm32(string code_name) : Instruction(code_name) {}

void AdcEaxImm32::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {  // 32bit op_size
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t cf = cpu.IsFlag(CF) ? 1 : 0;
  uint16_t ax = cpu.GetR16(EAX);
  uint16_t imm16 = memory.Read16(cpu.GetLinearAddrForCodeAccess());
  cpu.SetR16(EAX, cpu.Adc(ax, imm16, cf));
  cpu.AddEip(2);
  return;
}

RepeScasM32::RepeScasM32(string code_name) : Instruction(code_name) {}

void RepeScasM32::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  cpu.Rep(
      [&]() {
        if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
          uint32_t eax = cpu.GetR32(EAX);
          cpu.Scas(eax);
        } else {
          uint16_t ax = cpu.GetR16(EAX);
          cpu.Scas(ax);
        }
      },
      [&]() { return !cpu.IsFlag(ZF); });
  return;
}

Sahf::Sahf(string code_name) : Instruction(code_name) {}

void Sahf::Run(Cpu& cpu, Memory& memory) {
  uint8_t ah = cpu.GetR8H(EAX);
  uint32_t eflags = cpu.GetEflgs();
  uint32_t updated_eflags;
  cpu.AddEip(1);
  eflags = eflags & 0xFFFFFF00;
  updated_eflags = eflags | (ah & (SF | ZF | AF | PF | CF));
  cpu.SetEflgs(updated_eflags);
  return;
}

SalRm8::SalRm8(string code_name) : Instruction(code_name) {}

void SalRm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    this->obj->Error("Not implemented: op_size=32bit at %s::Run",
                     this->code_name.c_str());
  }
  uint16_t rm16 = this->GetRM16(cpu, memory);
  this->SetRM16(cpu, memory, cpu.Sal(rm16, (uint16_t)1));
  return;
}

JccRel8::JccRel8(string code_name) : Instruction(code_name) {}

void JccRel8::Run(Cpu& cpu, Memory& memory) {
  uint8_t jcc_type = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  bool condition;
  switch (jcc_type) {
    case 0x70:
      condition = cpu.IsFlag(OF);
      break;
    case 0x71:
      condition = !cpu.IsFlag(OF);
      break;
    case 0x72:
      condition = cpu.IsFlag(CF);
      break;
    case 0x73:
      condition = !cpu.IsFlag(CF);
      break;
    case 0x74:
      condition = cpu.IsFlag(ZF);
      break;
    case 0x75:
      condition = !cpu.IsFlag(ZF);
      break;
    case 0x76:
      condition = cpu.IsFlag(CF) || cpu.IsFlag(ZF);
      break;
    case 0x77:
      condition = (!cpu.IsFlag(CF)) && (!cpu.IsFlag(ZF));
      break;
    case 0x78:
      condition = cpu.IsFlag(SF);
      break;
    case 0x79:
      condition = !cpu.IsFlag(SF);
      break;
    case 0x7A:
      condition = cpu.IsFlag(PF);
      break;
    case 0x7B:
      condition = !cpu.IsFlag(PF);
      break;
    case 0x7C:
      condition = cpu.IsFlag(SF) != cpu.IsFlag(OF);
      break;
    case 0x7D:
      condition = cpu.IsFlag(SF) == cpu.IsFlag(OF);
      break;
    case 0x7E:
      condition = cpu.IsFlag(ZF) || (cpu.IsFlag(OF) != cpu.IsFlag(SF));
      break;
    case 0x7F:
      condition = (!cpu.IsFlag(ZF)) && (cpu.IsFlag(SF) == cpu.IsFlag(OF));
      break;
    default:
      this->obj->Error("Not implemented: jcc_type=0x%02X at %s::Run", jcc_type,
                       this->code_name.c_str());
  }
  cpu.AddEip(1);
  uint32_t rel8 =
      (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  if (condition) {
    cpu.AddEip(rel8);
  }
  return;
}

JccRel32::JccRel32(string code_name) : Instruction(code_name) {}

void JccRel32::Run(Cpu& cpu, Memory& memory) {
  uint8_t jcc_type = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  bool condition;
  // TODO : マジックナンバーをenum化する。
  switch (jcc_type) {
    case 0x80:
      condition = cpu.IsFlag(OF);
      break;
    case 0x81:
      condition = !cpu.IsFlag(OF);
      break;
    case 0x82:
      condition = cpu.IsFlag(CF);
      break;
    case 0x83:
      condition = !cpu.IsFlag(CF);
      break;
    case 0x84:
      condition = cpu.IsFlag(ZF);
      break;
    case 0x85:
      condition = !cpu.IsFlag(ZF);
      break;
    case 0x86:
      condition = cpu.IsFlag(ZF) || cpu.IsFlag(CF);
      break;
    case 0x87:
      condition = (!cpu.IsFlag(CF)) && (!cpu.IsFlag(ZF));
      break;
    case 0x88:
      condition = cpu.IsFlag(SF);
      break;
    case 0x89:
      condition = !cpu.IsFlag(SF);
      break;
    case 0x8A:
      condition = cpu.IsFlag(PF);
      break;
    case 0x8B:
      condition = !cpu.IsFlag(PF);
      break;
    case 0x8C:
      condition = cpu.IsFlag(SF) != cpu.IsFlag(OF);
      break;
    case 0x8D:
      condition = cpu.IsFlag(SF) == cpu.IsFlag(OF);
      break;
    case 0x8E:
      condition = cpu.IsFlag(ZF) || (cpu.IsFlag(OF) != cpu.IsFlag(SF));
      break;
    case 0x8F:
      condition = (!cpu.IsFlag(ZF)) && (cpu.IsFlag(SF) == cpu.IsFlag(OF));
      break;
    default:
      this->obj->Error("Not implemented: jcc_type=0x%02X at %s::Run", jcc_type,
                       this->code_name.c_str());
  }
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t rel32 = memory.Read32(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(4);
    if (condition) {
      cpu.AddEip(rel32);
    }
    return;
  }
  uint32_t rel16 =
      (int32_t)((int16_t)memory.Read16(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(2);
  if (condition) {
    cpu.AddEip(rel16);
  }
  return;
}

// TODO : LOOP系命令は1つにまとめる
LoopneRel8::LoopneRel8(string code_name) : Instruction(code_name) {}

void LoopneRel8::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  uint32_t count;
  //アドレスサイズによって、カウンタの値が決まる。
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixAddrSize()) {
    count = cpu.GetR32(ECX);
    count--;
    cpu.SetR32(ECX, count);
  } else {
    count = cpu.GetR16(ECX);
    count--;
    cpu.SetR16(ECX, count);
  }
  uint32_t rel8 =
      (int32_t)((int8_t)memory.Read8(cpu.GetLinearAddrForCodeAccess()));
  cpu.AddEip(1);
  if (count && !cpu.IsFlag(ZF)) {
    cpu.AddEip(rel8);
  }
  return;
}

LssR32M1632::LssR32M1632(string code_name) : Instruction(code_name) {}

void LssR32M1632::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr)));
    cpu.SetR16(
        SS, memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 4)));
    return;
  }
  uint32_t effective_addr = this->GetEffectiveAddr(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr)));
  cpu.SetR16(SS,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 2)));
  return;
}

LfsR32M1632::LfsR32M1632(string code_name) : Instruction(code_name) {}

void LfsR32M1632::Run(Cpu& cpu, Memory& memory) {
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr)));
    cpu.SetR16(
        FS, memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 4)));
    return;
  }
  uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr)));
  cpu.SetR16(FS,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 2)));
  return;
}

LgsR32M1632::LgsR32M1632(string code_name) : Instruction(code_name) {}

void LgsR32M1632::Run(Cpu& cpu, Memory& memory) {
  if (cpu.IsProtectedMode()) {
    this->obj->Error("Not implemented: protected mode at %s::Run",
                     this->code_name.c_str());
  }
  cpu.AddEip(1);
  this->ParseModRM(cpu, memory);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
    cpu.SetR32((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
               memory.Read32(cpu.GetLinearAddrForDataAccess(effective_addr)));
    cpu.SetR16(
        GS, memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 4)));
    return;
  }
  uint16_t effective_addr = this->GetEffectiveAddr(cpu, memory);
  cpu.SetR16((GENERAL_PURPOSE_REGISTER32)this->modrm.reg_index,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr)));
  cpu.SetR16(GS,
             memory.Read16(cpu.GetLinearAddrForDataAccess(effective_addr + 2)));
  return;
}

LldtRm16::LldtRm16(string code_name) : Instruction(code_name) {}

void LldtRm16::Run(Cpu& cpu, Memory& memory) {
  cpu.SetLdtr(this->GetRM16(cpu, memory));
  return;
}

SalRm8Imm8::SalRm8Imm8(string code_name) : Instruction(code_name) {}

void SalRm8Imm8::Run(Cpu& cpu, Memory& memory) {
  uint8_t rm8 = this->GetRM8(cpu, memory);
  uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
  cpu.AddEip(1);
  this->SetRM8(cpu, memory, cpu.Sal(rm8, (uint8_t)imm8));
  return;
}

RolRm32Imm8::RolRm32Imm8(string code_name) : Instruction(code_name) {}

void RolRm32Imm8::Run(Cpu& cpu, Memory& memory) {
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint8_t imm8 = memory.Read8(cpu.GetLinearAddrForCodeAccess());
    cpu.AddEip(1);
    uint32_t rm32 = this->GetRM32(cpu, memory);
    if (imm8 == 1) {
      this->obj->Error("Not implemented: update OF at %s::Run",
                       this->code_name.c_str());
    }
    for (uint8_t i = 0; i < imm8; i++) {
      if (rm32 & SIGN_FLG4) {
        cpu.SetFlag(CF);
      } else {
        cpu.ClearFlag(CF);
      }
      rm32 = rm32 << 1;
      rm32 = rm32 | (cpu.IsFlag(CF) ? 1 : 0);
    }
    this->SetRM32(cpu, memory, rm32);
    return;
  } else {
    this->obj->Error("Not implemented: op_size=16bit at %s::Run",
                     this->code_name.c_str());
  }
}

PopSegmentRegister::PopSegmentRegister(string code_name)
    : Instruction(code_name) {}

void PopSegmentRegister::Run(Cpu& cpu, Memory& memory) {
  SEGMENT_REGISTER segment_register;
  switch (memory.Read8(cpu.GetLinearAddrForCodeAccess())) {
    case 0x1F:
      segment_register = DS;
      break;
    case 0x07:
      segment_register = ES;
      break;
    case 0x17:
      segment_register = SS;
      break;
    case 0xA1:
      segment_register = FS;
      break;
    case 0xA9:
      segment_register = GS;
      break;
    default:
      this->obj->Error("Not supported: segment_register=%d at %s",
                       memory.Read8(cpu.GetLinearAddrForCodeAccess()),
                       this->code_name.c_str());
      break;
  }
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t data;
    data = cpu.Pop32();
    cpu.SetR16(segment_register, data);
    return;
  }
  uint16_t data;
  data = cpu.Pop16();
  cpu.SetR16(segment_register, data);
  return;
}

PushSegmentRegister::PushSegmentRegister(string code_name)
    : Instruction(code_name) {}

void PushSegmentRegister::Run(Cpu& cpu, Memory& memory) {
  SEGMENT_REGISTER segment_register;
  switch (memory.Read8(cpu.GetLinearAddrForCodeAccess())) {
    case 0x0E:
      segment_register = CS;
      break;
    case 0x1E:
      segment_register = DS;
      break;
    case 0x06:
      segment_register = ES;
      break;
    case 0x16:
      segment_register = SS;
      break;
    case 0xA0:
      segment_register = FS;
      break;
    case 0xA8:
      segment_register = GS;
      break;
    default:
      this->obj->Error("Not supported: segment_register=%d at %s",
                       memory.Read8(cpu.GetLinearAddrForCodeAccess()),
                       this->code_name.c_str());
      break;
  }
  cpu.AddEip(1);
  if (cpu.Is32bitsMode() ^ cpu.IsPrefixOpSize()) {
    uint32_t segment_register_value = (uint32_t)cpu.GetR16(segment_register);
    cpu.Push32(segment_register_value);
    return;
  }
  uint16_t segment_register_value = cpu.GetR16(segment_register);
  cpu.Push16(segment_register_value);
  return;
}