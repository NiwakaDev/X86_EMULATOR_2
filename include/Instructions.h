#include "Instruction.h"

// TODO : operator=とコピーコンストラクタをpriavteに宣言する。

class JmpRel8 : public Instruction {
 public:
  JmpRel8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovR32Imm32 : public Instruction {
 public:
  MovR32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovSregRm16 : public Instruction {
 public:
  MovSregRm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovR8Rm8 : public Instruction {
 public:
  MovR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeC0 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeC0(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeC6 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeC6(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code80 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  Code80(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code81 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  Code81(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code83 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  Code83(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeF7 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeF7(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeFE : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeFE(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeD2 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeD2(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeFF : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeFF(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeD0 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeD0(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeD1 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeD1(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeD3 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeD3(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeF6 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  CodeF6(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code0F00 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  Code0F00(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code0F01 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SET_SMALL_SIZE];

 public:
  Code0F01(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code0F : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SIZE];

 public:
  Code0F(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeC1 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SIZE];

 public:
  CodeC1(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddRm8Imm8 : public Instruction {
 public:
  AddRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddRm32Imm8 : public Instruction {
 public:
  AddRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpAlImm8 : public Instruction {
 public:
  CmpAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovR8Imm8 : public Instruction {
 public:
  MovR8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class IntImm8 : public Instruction {
 public:
  IntImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Hlt : public Instruction {
 public:
  Hlt(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovRm16Sreg : public Instruction {
 public:
  MovRm16Sreg(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddEaxImm32 : public Instruction {
 public:
  AddEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpRm8Imm8 : public Instruction {
 public:
  CmpRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovRm8R8 : public Instruction {
 public:
  MovRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JmpRel32 : public Instruction {
 public:
  JmpRel32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovRm8Imm8 : public Instruction {
 public:
  MovRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovRm32Imm32 : public Instruction {
 public:
  MovRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovMoffs8Al : public Instruction {
 public:
  MovMoffs8Al(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OutImm8 : public Instruction {
 public:
  OutImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Nop : public Instruction {
 public:
  Nop(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Cli : public Instruction {
 public:
  Cli(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CallRel32 : public Instruction {
 public:
  CallRel32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class InAlImm8 : public Instruction {
 public:
  InAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndAlImm8 : public Instruction {
 public:
  AndAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Ret32Near : public Instruction {
 public:
  Ret32Near(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Lgdt : public Instruction {
 public:
  Lgdt(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovR32CRX : public Instruction {
 public:
  MovR32CRX(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndEaxImm32 : public Instruction {
 public:
  AndEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrRm32Imm8 : public Instruction {
 public:
  OrRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovCRXR32 : public Instruction {
 public:
  MovCRXR32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovR32Rm32 : public Instruction {
 public:
  MovR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovRm32R32 : public Instruction {
 public:
  MovRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubRm32Imm8 : public Instruction {
 public:
  SubRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ImulR32Rm32Imm32 : public Instruction {
 public:
  ImulR32Rm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubRm32Imm32 : public Instruction {
 public:
  SubRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ShrRm32Imm8 : public Instruction {
 public:
  ShrRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JmpPtr1632 : public Instruction {
 public:
  JmpPtr1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushR32 : public Instruction {
 public:
  PushR32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PopR32 : public Instruction {
 public:
  PopR32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushImm8 : public Instruction {
 public:
  PushImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class IncR32 : public Instruction {
 public:
  IncR32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpRm32Imm32 : public Instruction {
 public:
  CmpRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndRm32Imm8 : public Instruction {
 public:
  AndRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorRm32R32 : public Instruction {
 public:
  XorRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RorRm8Cl : public Instruction {
 public:
  RorRm8Cl(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddRm32R32 : public Instruction {
 public:
  AddRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushImm32 : public Instruction {
 public:
  PushImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushFd : public Instruction {
 public:
  PushFd(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OutDxAl : public Instruction {
 public:
  OutDxAl(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpRm32R32 : public Instruction {
 public:
  CmpRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ShrRm8Imm8 : public Instruction {
 public:
  ShrRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovzxR32Rm8 : public Instruction {
 public:
  MovzxR32Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LeaR32M : public Instruction {
 public:
  LeaR32M(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PopFd : public Instruction {
 public:
  PopFd(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Leave : public Instruction {
 public:
  Leave(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpR32Rm32 : public Instruction {
 public:
  CmpR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ImulR32Rm32 : public Instruction {
 public:
  ImulR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddR32Rm32 : public Instruction {
 public:
  AddR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovsxR32Rm16 : public Instruction {
 public:
  MovsxR32Rm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushRm32 : public Instruction {
 public:
  PushRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class IncRm32 : public Instruction {
 public:
  IncRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class DecR32 : public Instruction {
 public:
  DecR32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class TestRm8R8 : public Instruction {
 public:
  TestRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpRm32Imm8 : public Instruction {
 public:
  CmpRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndRm32Imm32 : public Instruction {
 public:
  AndRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SalRm32Imm8 : public Instruction {
 public:
  SalRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovsxR32Rm8 : public Instruction {
 public:
  MovsxR32Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndR8Rm8 : public Instruction {
 public:
  AndR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XchgR32Rm32 : public Instruction {
 public:
  XchgR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class TestRm32R32 : public Instruction {
 public:
  TestRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class DivRm32 : public Instruction {
 public:
  DivRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class NotRm32 : public Instruction {
 public:
  NotRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubRm32R32 : public Instruction {
 public:
  SubRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SarRm32Imm8 : public Instruction {
 public:
  SarRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrRm32R32 : public Instruction {
 public:
  OrRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Lidt : public Instruction {
 public:
  Lidt(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Cdq : public Instruction {
 public:
  Cdq(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class IdivRm32 : public Instruction {
 public:
  IdivRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Sti : public Instruction {
 public:
  Sti(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushAd : public Instruction {
 public:
  PushAd(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class InAlDx : public Instruction {
 public:
  InAlDx(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class DecRm32 : public Instruction {
 public:
  DecRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PopAd : public Instruction {
 public:
  PopAd(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Iretd : public Instruction {
 public:
  Iretd(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovEaxMoffs32 : public Instruction {
 public:
  MovEaxMoffs32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovMoffs32Eax : public Instruction {
 public:
  MovMoffs32Eax(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubR32Rm32 : public Instruction {
 public:
  SubR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class DecRm8 : public Instruction {
 public:
  DecRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrRm32Imm32 : public Instruction {
 public:
  OrRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class NegRm32 : public Instruction {
 public:
  NegRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class TestEaxImm32 : public Instruction {
 public:
  TestEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrEaxImm32 : public Instruction {
 public:
  OrEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddRm32Imm32 : public Instruction {
 public:
  AddRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorRm32Imm8 : public Instruction {
 public:
  XorRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubEaxImm32 : public Instruction {
 public:
  SubEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ImulR32Rm32Imm8 : public Instruction {
 public:
  ImulR32Rm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SetneRm8 : public Instruction {
 public:
  SetneRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubRm8R8 : public Instruction {
 public:
  SubRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AdcRm8R8 : public Instruction {
 public:
  AdcRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Clc : public Instruction {
 public:
  Clc(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddR8Rm8 : public Instruction {
 public:
  AddR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SbbRm8R8 : public Instruction {
 public:
  SbbRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ShrRm32Cl : public Instruction {
 public:
  ShrRm32Cl(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Xlatb : public Instruction {
 public:
  Xlatb(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpRm8R8 : public Instruction {
 public:
  CmpRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpEaxImm32 : public Instruction {
 public:
  CmpEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class TestRm32Imm32 : public Instruction {
 public:
  TestRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LtrRm16 : public Instruction {
 public:
  LtrRm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndRm8R8 : public Instruction {
 public:
  AndRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JmpM1632 : public Instruction {
 public:
  JmpM1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SeteRm8 : public Instruction {
 public:
  SeteRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovAlMoffs8 : public Instruction {
 public:
  MovAlMoffs8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RcrRm8Imm8 : public Instruction {
 public:
  RcrRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SarRm8Imm8 : public Instruction {
 public:
  SarRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Cld : public Instruction {
 public:
  Cld(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpsM8M8 : public Instruction {
 public:
  CmpsM8M8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpsM32M32 : public Instruction {
 public:
  CmpsM32M32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SetaRm8 : public Instruction {
 public:
  SetaRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SetbRm8 : public Instruction {
 public:
  SetbRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class TestRm8Imm8 : public Instruction {
 public:
  TestRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovzxR32Rm16 : public Instruction {
 public:
  MovzxR32Rm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SbbR8Rm8 : public Instruction {
 public:
  SbbR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CallPtr1632 : public Instruction {
 public:
  CallPtr1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Ret32Far : public Instruction {
 public:
  Ret32Far(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CallM1632 : public Instruction {
 public:
  CallM1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CmpR8Rm8 : public Instruction {
 public:
  CmpR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MulRm32 : public Instruction {
 public:
  MulRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddRm8R8 : public Instruction {
 public:
  AddRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JmpRm32 : public Instruction {
 public:
  JmpRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ShrRm32 : public Instruction {
 public:
  ShrRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SalRm32Cl : public Instruction {
 public:
  SalRm32Cl(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SetgRm8 : public Instruction {
 public:
  SetgRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SarRm32Cl : public Instruction {
 public:
  SarRm32Cl(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndRm32R32 : public Instruction {
 public:
  AndRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SalRm32 : public Instruction {
 public:
  SalRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndR32Rm32 : public Instruction {
 public:
  AndR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ShrdRm32R32Imm8 : public Instruction {
 public:
  ShrdRm32R32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SarRm32 : public Instruction {
 public:
  SarRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SetgeRm8 : public Instruction {
 public:
  SetgeRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ScasM8 : public Instruction {
 public:
  ScasM8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CallRm32 : public Instruction {
 public:
  CallRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovM32M32 : public Instruction {
 public:
  MovM32M32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AdcRm32Imm8 : public Instruction {
 public:
  AdcRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Cwde : public Instruction {
 public:
  Cwde(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AdcRm32R32 : public Instruction {
 public:
  AdcRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LodsM8 : public Instruction {
 public:
  LodsM8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LesR32M1632 : public Instruction {
 public:
  LesR32M1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MulRm8 : public Instruction {
 public:
  MulRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XchgEaxR32 : public Instruction {
 public:
  XchgEaxR32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class DivRm8 : public Instruction {
 public:
  DivRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XchgRm8R8 : public Instruction {
 public:
  XchgRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RorRm8 : public Instruction {
 public:
  RorRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class IncRm8 : public Instruction {
 public:
  IncRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrRm8R8 : public Instruction {
 public:
  OrRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class StosM32 : public Instruction {
 public:
  StosM32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LodsM32 : public Instruction {
 public:
  LodsM32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AndRm8Imm8 : public Instruction {
 public:
  AndRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AdcR32Rm32 : public Instruction {
 public:
  AdcR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeF3 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SIZE];

 public:
  CodeF3(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepeCmpsM8M8 : public Instruction {
 public:
  RepeCmpsM8M8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class CodeF2 : public Instruction {
 private:
  std::unique_ptr<Instruction>
      instructions[InstructionHelper::INSTRUCTION_SIZE];

 public:
  CodeF2(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepneScasM8 : public Instruction {
 public:
  RepneScasM8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AddAlImm8 : public Instruction {
 public:
  AddAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrR8Rm8 : public Instruction {
 public:
  OrR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrR32Rm32 : public Instruction {
 public:
  OrR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrAlImm8 : public Instruction {
 public:
  OrAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SbbRm32R32 : public Instruction {
 public:
  SbbRm32R32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SbbR32Rm32 : public Instruction {
 public:
  SbbR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubR8Rm8 : public Instruction {
 public:
  SubR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubAlImm8 : public Instruction {
 public:
  SubAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorRm8R8 : public Instruction {
 public:
  XorRm8R8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorR8Rm8 : public Instruction {
 public:
  XorR8Rm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorR32Rm32 : public Instruction {
 public:
  XorR32Rm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class OrRm8Imm8 : public Instruction {
 public:
  OrRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SubRm8Imm8 : public Instruction {
 public:
  SubRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorRm8Imm8 : public Instruction {
 public:
  XorRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorRm32Imm32 : public Instruction {
 public:
  XorRm32Imm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SbbRm32Imm8 : public Instruction {
 public:
  SbbRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PopM32 : public Instruction {
 public:
  PopM32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Lahf : public Instruction {
 public:
  Lahf(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class MovsM8M8 : public Instruction {
 public:
  MovsM8M8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class TestAlImm8 : public Instruction {
 public:
  TestAlImm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class StosM8 : public Instruction {
 public:
  StosM8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ScasD : public Instruction {
 public:
  ScasD(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RclRm32Imm8 : public Instruction {
 public:
  RclRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RetImm16 : public Instruction {
 public:
  RetImm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LdsR32M1632 : public Instruction {
 public:
  LdsR32M1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RetFarImm16 : public Instruction {
 public:
  RetFarImm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RclRm32 : public Instruction {
 public:
  RclRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SalRm8Cl : public Instruction {
 public:
  SalRm8Cl(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LoopeRel8 : public Instruction {
 public:
  LoopeRel8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LoopRel8 : public Instruction {
 public:
  LoopRel8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JcxzRel8 : public Instruction {
 public:
  JcxzRel8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepMovsM8M8 : public Instruction {
 public:
  RepMovsM8M8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepMovsM32M32 : public Instruction {
 public:
  RepMovsM32M32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepStosM8 : public Instruction {
 public:
  RepStosM8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepStosM32 : public Instruction {
 public:
  RepStosM32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class NotRm8 : public Instruction {
 public:
  NotRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ImulRm16 : public Instruction {
 public:
  ImulRm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Stc : public Instruction {
 public:
  Stc(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Std : public Instruction {
 public:
  Std(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class ShrRm8 : public Instruction {
 public:
  ShrRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AdcRm8Imm8 : public Instruction {
 public:
  AdcRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class XorEaxImm32 : public Instruction {
 public:
  XorEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepeCmpsM32M32 : public Instruction {
 public:
  RepeCmpsM32M32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepeScasM8 : public Instruction {
 public:
  RepeScasM8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RcrRm32 : public Instruction {
 public:
  RcrRm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class AdcEaxImm32 : public Instruction {
 public:
  AdcEaxImm32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RepeScasM32 : public Instruction {
 public:
  RepeScasM32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Sahf : public Instruction {
 public:
  Sahf(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SalRm8 : public Instruction {
 public:
  SalRm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JccRel32 : public Instruction {
 public:
  JccRel32(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LoopneRel8 : public Instruction {
 public:
  LoopneRel8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class Code66 : public Instruction {
 public:
  Code66(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LssR32M1632 : public Instruction {
 public:
  LssR32M1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LfsR32M1632 : public Instruction {
 public:
  LfsR32M1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LgsR32M1632 : public Instruction {
 public:
  LgsR32M1632(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class LldtRm16 : public Instruction {
 public:
  LldtRm16(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class SalRm8Imm8 : public Instruction {
 public:
  SalRm8Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class RolRm32Imm8 : public Instruction {
 public:
  RolRm32Imm8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PopSegmentRegister : public Instruction {
 public:
  PopSegmentRegister(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class PushSegmentRegister : public Instruction {
 public:
  PushSegmentRegister(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};

class JccRel8 : public Instruction {
 public:
  JccRel8(std::string code_name);
  void Run(Cpu& cpu, Memory& memory);
};