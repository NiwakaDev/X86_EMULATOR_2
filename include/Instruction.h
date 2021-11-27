#pragma once
#include "common.h"
#include "ModRM.h"
#include "Sib.h"

class Cpu;
class Memory;
class Sib;
class IoPort;

#define INSTRUCTION_SET_SMALL_SIZE 8
#define INSTRUCTION_SIZE 256

class Instruction:public Object{
    protected:
        Sib sib;
        ModRM modrm;
        void ParseModRM(Cpu *cpu, Memory* mem);
        void ParseModRM_32bitsMode(Cpu* cpu, Memory* mem);
        void ParseModRM_16bitsMode(Cpu* cpu, Memory* mem);
        void ParseRegIdx(Cpu* cpu, Memory* mem);
        uint8_t GetRM8(Cpu* cpu, Memory* mem);
        uint16_t GetRM16(Cpu* cpu, Memory* mem);
        uint32_t GetRM32(Cpu* cpu, Memory* mem);
        uint16_t GetR16ForEffectiveAddr(Cpu* cpu);
        uint32_t GetEffectiveAddr(Cpu* cpu, Memory* mem);
        void SetRM8(Cpu* cpu, Memory* mem, uint8_t data);
        void SetRM16(Cpu* cpu, Memory* mem, uint16_t data);
        void SetRM32(Cpu* cpu, Memory* mem, uint32_t data);
        void Push16(Cpu* cpu, Memory* mem, uint16_t data);
        void Push32(Cpu* cpu, Memory* mem, uint32_t data);
        uint16_t Pop16(Cpu* cpu, Memory* mem);
        uint32_t Pop32(Cpu* cpu, Memory* mem);
    public:
        string code_name;
        virtual void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
        void Show();
        Instruction(string code_name);
        void SetModRM(ModRM modrm, Sib* sib);
};

class JmpRel8:public Instruction{
    public: 
        JmpRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR32Imm32:public Instruction{
    public: 
        MovR32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovSregRm16:public Instruction{
    public: 
        MovSregRm16(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR8Rm8:public Instruction{
    public: 
        MovR8Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeC0:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeC0(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeC6:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeC6(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code80:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code80(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code81:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code81(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code83:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code83(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeF7:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeF7(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeFE:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeFE(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD2:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD2(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeFF:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeFF(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD1:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD1(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD3:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD3(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeF6:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeF6(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code0F00:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code0F00(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code0F01:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code0F01(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code0F:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SIZE];
    public: 
        Code0F(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeC1:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SIZE];
    public: 
        CodeC1(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm8Imm8:public Instruction{
    public: 
        AddRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm32Imm8:public Instruction{
    public: 
        AddRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpAlImm8:public Instruction{
    public: 
        CmpAlImm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JaeRel8:public Instruction{
    public: 
        JaeRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JzRel8:public Instruction{
    public: 
        JzRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR8Imm8:public Instruction{
    public: 
        MovR8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IntImm8:public Instruction{
    public: 
        IntImm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Hlt:public Instruction{
    public: 
        Hlt(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm16Sreg:public Instruction{
    public: 
        MovRm16Sreg(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddEaxImm32:public Instruction{
    public: 
        AddEaxImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm8Imm8:public Instruction{
    public: 
        CmpRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JbeRel8:public Instruction{
    public: 
        JbeRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnzRel8:public Instruction{
    public: 
        JnzRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JcRel8:public Instruction{
    public: 
        JcRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm8R8:public Instruction{
    public: 
        MovRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpRel32:public Instruction{
    public: 
        JmpRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm8Imm8:public Instruction{
    public: 
        MovRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm32Imm32:public Instruction{
    public: 
        MovRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovMoffs8Al:public Instruction{
    public: 
        MovMoffs8Al(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OutImm8:public Instruction{
    public: 
        OutImm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Nop:public Instruction{
    public: 
        Nop(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cli:public Instruction{
    public: 
        Cli(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallRel32:public Instruction{
    public: 
        CallRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class InAlImm8:public Instruction{
    public: 
        InAlImm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndAlImm8:public Instruction{
    public: 
        AndAlImm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Ret32Near:public Instruction{
    public: 
        Ret32Near(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Lgdt:public Instruction{
    public: 
        Lgdt(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR32CRX:public Instruction{
    public: 
        MovR32CRX(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndEaxImm32:public Instruction{
    public: 
        AndEaxImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm32Imm8:public Instruction{
    public: 
        OrRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovCRXR32:public Instruction{
    public: 
        MovCRXR32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR32Rm32:public Instruction{
    public: 
        MovR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm32R32:public Instruction{
    public: 
        MovRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm32Imm8:public Instruction{
    public: 
        SubRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ImulR32Rm32Imm32:public Instruction{
    public: 
        ImulR32Rm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm32Imm32:public Instruction{
    public: 
        SubRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm32Imm8:public Instruction{
    public: 
        ShrRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpPtr1632:public Instruction{
    public: 
        JmpPtr1632(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushR32:public Instruction{
    public: 
        PushR32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopR32:public Instruction{
    public: 
        PopR32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushImm8:public Instruction{
    public: 
        PushImm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IncR32:public Instruction{
    public: 
        IncR32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm32Imm32:public Instruction{
    public: 
        CmpRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JleRel8:public Instruction{
    public: 
        JleRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm32Imm8:public Instruction{
    public: 
        AndRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm32R32:public Instruction{
    public: 
        XorRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RorRm8Cl:public Instruction{
    public: 
        RorRm8Cl(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm32R32:public Instruction{
    public: 
        AddRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushImm32:public Instruction{
    public: 
        PushImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushFd:public Instruction{
    public: 
        PushFd(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OutDxAl:public Instruction{
    public: 
        OutDxAl(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm32R32:public Instruction{
    public: 
        CmpRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm8Imm8:public Instruction{
    public: 
        ShrRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovzxR32Rm8:public Instruction{
    public: 
        MovzxR32Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LeaR32M:public Instruction{
    public: 
        LeaR32M(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopFd:public Instruction{
    public: 
        PopFd(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Leave:public Instruction{
    public: 
        Leave(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpR32Rm32:public Instruction{
    public: 
        CmpR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgRel8:public Instruction{
    public: 
        JgRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ImulR32Rm32:public Instruction{
    public: 
        ImulR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddR32Rm32:public Instruction{
    public: 
        AddR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovsxR32Rm16:public Instruction{
    public: 
        MovsxR32Rm16(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushRm32:public Instruction{
    public: 
        PushRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IncRm32:public Instruction{
    public: 
        IncRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DecR32:public Instruction{
    public: 
        DecR32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm8R8:public Instruction{
    public: 
        TestRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnsRel8:public Instruction{
    public: 
        JnsRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm32Imm8:public Instruction{
    public: 
        CmpRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm32Imm32:public Instruction{
    public: 
        AndRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm32Imm8:public Instruction{
    public: 
        SalRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovsxR32Rm8:public Instruction{
    public: 
        MovsxR32Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndR8Rm8:public Instruction{
    public: 
        AndR8Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XchgR32Rm32:public Instruction{
    public: 
        XchgR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JneRel32:public Instruction{
    public: 
        JneRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JeRel32:public Instruction{
    public: 
        JeRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnaRel32:public Instruction{
    public: 
        JnaRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm32R32:public Instruction{
    public: 
        TestRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JsRel8:public Instruction{
    public: 
        JsRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DivRm32:public Instruction{
    public: 
        DivRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class NotRm32:public Instruction{
    public: 
        NotRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgeRel8:public Instruction{
    public: 
        JgeRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm32R32:public Instruction{
    public: 
        SubRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JleRel32:public Instruction{
    public: 
        JleRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm32Imm8:public Instruction{
    public: 
        SarRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm32R32:public Instruction{
    public: 
        OrRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Lidt:public Instruction{
    public: 
        Lidt(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cdq:public Instruction{
    public: 
        Cdq(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IdivRm32:public Instruction{
    public: 
        IdivRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JlRel8:public Instruction{
    public: 
        JlRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Sti:public Instruction{
    public: 
        Sti(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushEs:public Instruction{
    public: 
        PushEs(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushDs:public Instruction{
    public: 
        PushDs(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushAd:public Instruction{
    public: 
        PushAd(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class InAlDx:public Instruction{
    public: 
        InAlDx(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DecRm32:public Instruction{
    public: 
        DecRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JaRel8:public Instruction{
    public: 
        JaRel8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopAd:public Instruction{
    public: 
        PopAd(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopDs:public Instruction{
    public: 
        PopDs(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopEs:public Instruction{
    public: 
        PopEs(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Iretd:public Instruction{
    public: 
        Iretd(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovEaxMoffs32:public Instruction{
    public: 
        MovEaxMoffs32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovMoffs32Eax:public Instruction{
    public: 
        MovMoffs32Eax(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubR32Rm32:public Instruction{
    public: 
        SubR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DecRm8:public Instruction{
    public: 
        DecRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm32Imm32:public Instruction{
    public: 
        OrRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class NegRm32:public Instruction{
    public: 
        NegRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestEaxImm32:public Instruction{
    public: 
        TestEaxImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JsRel32:public Instruction{
    public: 
        JsRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrEaxImm32:public Instruction{
    public: 
        OrEaxImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm32Imm32:public Instruction{
    public: 
        AddRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm32Imm8:public Instruction{
    public: 
        XorRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubEaxImm32:public Instruction{
    public: 
        SubEaxImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgRel32:public Instruction{
    public: 
        JgRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ImulR32Rm32Imm8:public Instruction{
    public: 
        ImulR32Rm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetneRm8:public Instruction{
    public: 
        SetneRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm8R8:public Instruction{
    public: 
        SubRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AdcRm8R8:public Instruction{
    public: 
        AdcRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Clc:public Instruction{
    public: 
        Clc(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddR8Rm8:public Instruction{
    public: 
        AddR8Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbRm8R8:public Instruction{
    public: 
        SbbRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm32Cl:public Instruction{
    public: 
        ShrRm32Cl(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Xlatb:public Instruction{
    public: 
        Xlatb(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm8R8:public Instruction{
    public: 
        CmpRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpEaxImm32:public Instruction{
    public: 
        CmpEaxImm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm32Imm32:public Instruction{
    public: 
        TestRm32Imm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LtrRm16:public Instruction{
    public: 
        LtrRm16(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm8R8:public Instruction{
    public: 
        AndRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpM1632:public Instruction{
    public: 
        JmpM1632(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SeteRm8:public Instruction{
    public: 
        SeteRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovAlMoffs8:public Instruction{
    public: 
        MovAlMoffs8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RcrRm8Imm8:public Instruction{
    public: 
        RcrRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm8Imm8:public Instruction{
    public: 
        SarRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cld:public Instruction{
    public: 
        Cld(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpsM8M8:public Instruction{
    public: 
        CmpsM8M8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetaRm8:public Instruction{
    public: 
        SetaRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetbRm8:public Instruction{
    public: 
        SetbRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm8Imm8:public Instruction{
    public: 
        TestRm8Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovzxR32Rm16:public Instruction{
    public: 
        MovzxR32Rm16(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbR8Rm8:public Instruction{
    public: 
        SbbR8Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgeRel32:public Instruction{
    public: 
        JgeRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallPtr1632:public Instruction{
    public: 
        CallPtr1632(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Ret32Far:public Instruction{
    public: 
        Ret32Far(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallM1632:public Instruction{
    public: 
        CallM1632(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushSs:public Instruction{
    public: 
        PushSs(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpR8Rm8:public Instruction{
    public: 
        CmpR8Rm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MulRm32:public Instruction{
    public: 
        MulRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnsRel32:public Instruction{
    public: 
        JnsRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JaRel32:public Instruction{
    public: 
        JaRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JlRel32:public Instruction{
    public: 
        JlRel32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm8R8:public Instruction{
    public: 
        AddRm8R8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpRm32:public Instruction{
    public: 
        JmpRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm32:public Instruction{
    public: 
        ShrRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm32Cl:public Instruction{
    public: 
        SalRm32Cl(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetgRm8:public Instruction{
    public: 
        SetgRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm32Cl:public Instruction{
    public: 
        SarRm32Cl(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm32R32:public Instruction{
    public: 
        AndRm32R32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm32:public Instruction{
    public: 
        SalRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndR32Rm32:public Instruction{
    public: 
        AndR32Rm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrdRm32R32Imm8:public Instruction{
    public: 
        ShrdRm32R32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm32:public Instruction{
    public: 
        SarRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetgeRm8:public Instruction{
    public: 
        SetgeRm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Scasb:public Instruction{
    public: 
        Scasb(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallRm32:public Instruction{
    public: 
        CallRm32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovM32M32:public Instruction{
    public: 
        MovM32M32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AdcRm32Imm8:public Instruction{
    public: 
        AdcRm32Imm8(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cwde:public Instruction{
    public: 
        Cwde(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};
/***
class PopM32:public Instruction{
    public: 
        PopM32(string code_name);
        inline void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};
***/