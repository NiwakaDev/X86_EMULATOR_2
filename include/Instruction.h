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
        //template<typename type>void Push(Cpu* cpu, Memory* mem, type data);
        uint8_t  Pop8(Cpu* cpu, Memory* mem);
        uint16_t Pop16(Cpu* cpu, Memory* mem);
        uint32_t Pop32(Cpu* cpu, Memory* mem);
    public:
        std::string code_name;
        virtual void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
        void Show();
        Instruction(std::string code_name);
        void SetModRM(ModRM modrm, Sib* sib);
};

class JmpRel8:public Instruction{
    public: 
        JmpRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR32Imm32:public Instruction{
    public: 
        MovR32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovSregRm16:public Instruction{
    public: 
        MovSregRm16(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR8Rm8:public Instruction{
    public: 
        MovR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeC0:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeC0(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeC6:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeC6(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code80:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code80(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code81:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code81(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code83:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code83(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeF7:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeF7(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeFE:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeFE(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD2:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD2(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeFF:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeFF(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD0:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD0(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD1:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD1(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeD3:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeD3(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeF6:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        CodeF6(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code0F00:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code0F00(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code0F01:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SET_SMALL_SIZE];
    public: 
        Code0F01(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Code0F:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SIZE];
    public: 
        Code0F(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeC1:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SIZE];
    public: 
        CodeC1(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm8Imm8:public Instruction{
    public: 
        AddRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm32Imm8:public Instruction{
    public: 
        AddRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpAlImm8:public Instruction{
    public: 
        CmpAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JaeRel8:public Instruction{
    public: 
        JaeRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JzRel8:public Instruction{
    public: 
        JzRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR8Imm8:public Instruction{
    public: 
        MovR8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IntImm8:public Instruction{
    public: 
        IntImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Hlt:public Instruction{
    public: 
        Hlt(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm16Sreg:public Instruction{
    public: 
        MovRm16Sreg(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddEaxImm32:public Instruction{
    public: 
        AddEaxImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm8Imm8:public Instruction{
    public: 
        CmpRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JbeRel8:public Instruction{
    public: 
        JbeRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnzRel8:public Instruction{
    public: 
        JnzRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JcRel8:public Instruction{
    public: 
        JcRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm8R8:public Instruction{
    public: 
        MovRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpRel32:public Instruction{
    public: 
        JmpRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm8Imm8:public Instruction{
    public: 
        MovRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm32Imm32:public Instruction{
    public: 
        MovRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovMoffs8Al:public Instruction{
    public: 
        MovMoffs8Al(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OutImm8:public Instruction{
    public: 
        OutImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Nop:public Instruction{
    public: 
        Nop(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cli:public Instruction{
    public: 
        Cli(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallRel32:public Instruction{
    public: 
        CallRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class InAlImm8:public Instruction{
    public: 
        InAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndAlImm8:public Instruction{
    public: 
        AndAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Ret32Near:public Instruction{
    public: 
        Ret32Near(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Lgdt:public Instruction{
    public: 
        Lgdt(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR32CRX:public Instruction{
    public: 
        MovR32CRX(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndEaxImm32:public Instruction{
    public: 
        AndEaxImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm32Imm8:public Instruction{
    public: 
        OrRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovCRXR32:public Instruction{
    public: 
        MovCRXR32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovR32Rm32:public Instruction{
    public: 
        MovR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovRm32R32:public Instruction{
    public: 
        MovRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm32Imm8:public Instruction{
    public: 
        SubRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ImulR32Rm32Imm32:public Instruction{
    public: 
        ImulR32Rm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm32Imm32:public Instruction{
    public: 
        SubRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm32Imm8:public Instruction{
    public: 
        ShrRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpPtr1632:public Instruction{
    public: 
        JmpPtr1632(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushR32:public Instruction{
    public: 
        PushR32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopR32:public Instruction{
    public: 
        PopR32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushImm8:public Instruction{
    public: 
        PushImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IncR32:public Instruction{
    public: 
        IncR32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm32Imm32:public Instruction{
    public: 
        CmpRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JleRel8:public Instruction{
    public: 
        JleRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm32Imm8:public Instruction{
    public: 
        AndRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm32R32:public Instruction{
    public: 
        XorRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RorRm8Cl:public Instruction{
    public: 
        RorRm8Cl(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm32R32:public Instruction{
    public: 
        AddRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushImm32:public Instruction{
    public: 
        PushImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushFd:public Instruction{
    public: 
        PushFd(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OutDxAl:public Instruction{
    public: 
        OutDxAl(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm32R32:public Instruction{
    public: 
        CmpRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm8Imm8:public Instruction{
    public: 
        ShrRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovzxR32Rm8:public Instruction{
    public: 
        MovzxR32Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LeaR32M:public Instruction{
    public: 
        LeaR32M(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopFd:public Instruction{
    public: 
        PopFd(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Leave:public Instruction{
    public: 
        Leave(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpR32Rm32:public Instruction{
    public: 
        CmpR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgRel8:public Instruction{
    public: 
        JgRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ImulR32Rm32:public Instruction{
    public: 
        ImulR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddR32Rm32:public Instruction{
    public: 
        AddR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovsxR32Rm16:public Instruction{
    public: 
        MovsxR32Rm16(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushRm32:public Instruction{
    public: 
        PushRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IncRm32:public Instruction{
    public: 
        IncRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DecR32:public Instruction{
    public: 
        DecR32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm8R8:public Instruction{
    public: 
        TestRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnsRel8:public Instruction{
    public: 
        JnsRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm32Imm8:public Instruction{
    public: 
        CmpRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm32Imm32:public Instruction{
    public: 
        AndRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm32Imm8:public Instruction{
    public: 
        SalRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovsxR32Rm8:public Instruction{
    public: 
        MovsxR32Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndR8Rm8:public Instruction{
    public: 
        AndR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XchgR32Rm32:public Instruction{
    public: 
        XchgR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JneRel32:public Instruction{
    public: 
        JneRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JeRel32:public Instruction{
    public: 
        JeRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnaRel32:public Instruction{
    public: 
        JnaRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm32R32:public Instruction{
    public: 
        TestRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JsRel8:public Instruction{
    public: 
        JsRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DivRm32:public Instruction{
    public: 
        DivRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class NotRm32:public Instruction{
    public: 
        NotRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgeRel8:public Instruction{
    public: 
        JgeRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm32R32:public Instruction{
    public: 
        SubRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JleRel32:public Instruction{
    public: 
        JleRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm32Imm8:public Instruction{
    public: 
        SarRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm32R32:public Instruction{
    public: 
        OrRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Lidt:public Instruction{
    public: 
        Lidt(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cdq:public Instruction{
    public: 
        Cdq(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IdivRm32:public Instruction{
    public: 
        IdivRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JlRel8:public Instruction{
    public: 
        JlRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Sti:public Instruction{
    public: 
        Sti(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushEs:public Instruction{
    public: 
        PushEs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushCs:public Instruction{
    public: 
        PushCs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushDs:public Instruction{
    public: 
        PushDs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushAd:public Instruction{
    public: 
        PushAd(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class InAlDx:public Instruction{
    public: 
        InAlDx(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DecRm32:public Instruction{
    public: 
        DecRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JaRel8:public Instruction{
    public: 
        JaRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopAd:public Instruction{
    public: 
        PopAd(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopDs:public Instruction{
    public: 
        PopDs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopEs:public Instruction{
    public: 
        PopEs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Iretd:public Instruction{
    public: 
        Iretd(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovEaxMoffs32:public Instruction{
    public: 
        MovEaxMoffs32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovMoffs32Eax:public Instruction{
    public: 
        MovMoffs32Eax(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubR32Rm32:public Instruction{
    public: 
        SubR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DecRm8:public Instruction{
    public: 
        DecRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm32Imm32:public Instruction{
    public: 
        OrRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class NegRm32:public Instruction{
    public: 
        NegRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestEaxImm32:public Instruction{
    public: 
        TestEaxImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JsRel32:public Instruction{
    public: 
        JsRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrEaxImm32:public Instruction{
    public: 
        OrEaxImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm32Imm32:public Instruction{
    public: 
        AddRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm32Imm8:public Instruction{
    public: 
        XorRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubEaxImm32:public Instruction{
    public: 
        SubEaxImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgRel32:public Instruction{
    public: 
        JgRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ImulR32Rm32Imm8:public Instruction{
    public: 
        ImulR32Rm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetneRm8:public Instruction{
    public: 
        SetneRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm8R8:public Instruction{
    public: 
        SubRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AdcRm8R8:public Instruction{
    public: 
        AdcRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Clc:public Instruction{
    public: 
        Clc(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddR8Rm8:public Instruction{
    public: 
        AddR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbRm8R8:public Instruction{
    public: 
        SbbRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm32Cl:public Instruction{
    public: 
        ShrRm32Cl(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Xlatb:public Instruction{
    public: 
        Xlatb(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpRm8R8:public Instruction{
    public: 
        CmpRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpEaxImm32:public Instruction{
    public: 
        CmpEaxImm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm32Imm32:public Instruction{
    public: 
        TestRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LtrRm16:public Instruction{
    public: 
        LtrRm16(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm8R8:public Instruction{
    public: 
        AndRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpM1632:public Instruction{
    public: 
        JmpM1632(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SeteRm8:public Instruction{
    public: 
        SeteRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovAlMoffs8:public Instruction{
    public: 
        MovAlMoffs8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RcrRm8Imm8:public Instruction{
    public: 
        RcrRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm8Imm8:public Instruction{
    public: 
        SarRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cld:public Instruction{
    public: 
        Cld(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpsM8M8:public Instruction{
    public: 
        CmpsM8M8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetaRm8:public Instruction{
    public: 
        SetaRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetbRm8:public Instruction{
    public: 
        SetbRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestRm8Imm8:public Instruction{
    public: 
        TestRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovzxR32Rm16:public Instruction{
    public: 
        MovzxR32Rm16(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbR8Rm8:public Instruction{
    public: 
        SbbR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JgeRel32:public Instruction{
    public: 
        JgeRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallPtr1632:public Instruction{
    public: 
        CallPtr1632(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Ret32Far:public Instruction{
    public: 
        Ret32Far(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallM1632:public Instruction{
    public: 
        CallM1632(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PushSs:public Instruction{
    public: 
        PushSs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CmpR8Rm8:public Instruction{
    public: 
        CmpR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MulRm32:public Instruction{
    public: 
        MulRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JnsRel32:public Instruction{
    public: 
        JnsRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JaRel32:public Instruction{
    public: 
        JaRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JlRel32:public Instruction{
    public: 
        JlRel32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddRm8R8:public Instruction{
    public: 
        AddRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JmpRm32:public Instruction{
    public: 
        JmpRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrRm32:public Instruction{
    public: 
        ShrRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm32Cl:public Instruction{
    public: 
        SalRm32Cl(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetgRm8:public Instruction{
    public: 
        SetgRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm32Cl:public Instruction{
    public: 
        SarRm32Cl(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm32R32:public Instruction{
    public: 
        AndRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm32:public Instruction{
    public: 
        SalRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndR32Rm32:public Instruction{
    public: 
        AndR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ShrdRm32R32Imm8:public Instruction{
    public: 
        ShrdRm32R32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SarRm32:public Instruction{
    public: 
        SarRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SetgeRm8:public Instruction{
    public: 
        SetgeRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ScasM8:public Instruction{
    public: 
        ScasM8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CallRm32:public Instruction{
    public: 
        CallRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovM32M32:public Instruction{
    public: 
        MovM32M32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AdcRm32Imm8:public Instruction{
    public: 
        AdcRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Cwde:public Instruction{
    public: 
        Cwde(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AdcRm32R32:public Instruction{
    public: 
        AdcRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LodsM8:public Instruction{
    public: 
        LodsM8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LesR32M1632:public Instruction{
    public: 
        LesR32M1632(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MulRm8:public Instruction{
    public: 
        MulRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XchgEaxR32:public Instruction{
    public: 
        XchgEaxR32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class DivRm8:public Instruction{
    public: 
        DivRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XchgRm8R8:public Instruction{
    public: 
        XchgRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RorRm8:public Instruction{
    public: 
        RorRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class IncRm8:public Instruction{
    public: 
        IncRm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm8R8:public Instruction{
    public: 
        OrRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class StosM32:public Instruction{
    public: 
        StosM32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LodsM32:public Instruction{
    public: 
        LodsM32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AndRm8Imm8:public Instruction{
    public: 
        AndRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AdcR32Rm32:public Instruction{
    public: 
        AdcR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeF3:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SIZE];
    public: 
        CodeF3(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RepeCmpsM8M8:public Instruction{
    public: 
        RepeCmpsM8M8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class CodeF2:public Instruction{
    private:
        Instruction* instructions[INSTRUCTION_SIZE];
    public: 
        CodeF2(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};
class RepneScasM8:public Instruction{
    public: 
        RepneScasM8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class AddAlImm8:public Instruction{
    public: 
        AddAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrR8Rm8:public Instruction{
    public: 
        OrR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrR32Rm32:public Instruction{
    public: 
        OrR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrAlImm8:public Instruction{
    public: 
        OrAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopSs:public Instruction{
    public: 
        PopSs(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbRm32R32:public Instruction{
    public: 
        SbbRm32R32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbR32Rm32:public Instruction{
    public: 
        SbbR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubR8Rm8:public Instruction{
    public: 
        SubR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubAlImm8:public Instruction{
    public: 
        SubAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm8R8:public Instruction{
    public: 
        XorRm8R8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorR8Rm8:public Instruction{
    public: 
        XorR8Rm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorR32Rm32:public Instruction{
    public: 
        XorR32Rm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class OrRm8Imm8:public Instruction{
    public: 
        OrRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SubRm8Imm8:public Instruction{
    public: 
        SubRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm8Imm8:public Instruction{
    public: 
        XorRm8Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class XorRm32Imm32:public Instruction{
    public: 
        XorRm32Imm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SbbRm32Imm8:public Instruction{
    public: 
        SbbRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class PopM32:public Instruction{
    public: 
        PopM32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class Lahf:public Instruction{
    public: 
        Lahf(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class MovsM8M8:public Instruction{
    public: 
        MovsM8M8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class TestAlImm8:public Instruction{
    public: 
        TestAlImm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class StosM8:public Instruction{
    public: 
        StosM8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class ScasD:public Instruction{
    public: 
        ScasD(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RclRm32Imm8:public Instruction{
    public: 
        RclRm32Imm8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RetImm16:public Instruction{
    public: 
        RetImm16(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LdsR32M1632:public Instruction{
    public: 
        LdsR32M1632(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RetFarImm16:public Instruction{
    public: 
        RetFarImm16(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RclRm32:public Instruction{
    public: 
        RclRm32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class SalRm8Cl:public Instruction{
    public: 
        SalRm8Cl(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LoopeRel8:public Instruction{
    public: 
        LoopeRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class LoopRel8:public Instruction{
    public: 
        LoopRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class JcxzRel8:public Instruction{
    public: 
        JcxzRel8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RepMovsM8M8:public Instruction{
    public: 
        RepMovsM8M8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RepMovsM32M32:public Instruction{
    public: 
        RepMovsM32M32(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};

class RepStosM8:public Instruction{
    public: 
        RepStosM8(std::string code_name);
        void Run(Cpu* cpu, Memory* mem, IoPort* io_port);
};