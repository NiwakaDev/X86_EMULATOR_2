#pragma once
#include "common.h"
#include "ModRM.h"
#include "Sib.h"
#include "InstructionHelper.h"

class Sib;
class Emulator;

class Instruction:public Object{
    protected:
        Sib sib;
        ModRM modrm;
        void ParseModRM(const Emulator& emu);
        void ParseRegIdx(const Emulator& emu);
        uint8_t GetRM8(const Emulator& emu);
        uint16_t GetRM16(const Emulator& emu);
        uint32_t GetRM32(const Emulator& emu);
        uint16_t GetR16ForEffectiveAddr(const Emulator& emu);
        uint32_t GetEffectiveAddr(const Emulator& emu);
        void SetRM8(const Emulator& emu, const uint8_t data);
        void SetRM16(const Emulator& emu, const uint16_t data);
        void SetRM32(const Emulator& emu, const uint32_t data);
        //TODO:SetRMをtemplate化
        //SetRM8やSetRM16のdataに4byteサイズを指定している場合もあり、現時点ではバグの原因になります。
        //template<typename type> void SetRM(const Emulator& emu, const type data);
        std::string code_name;
    public:
        virtual void Run(const Emulator& emu) = 0;
        Instruction(std::string code_name);
        virtual ~Instruction();
        std::string& GetInstructionName(){
            return this->code_name;
        }
        void SetModRM(ModRM* modrm, Sib* sib){
            this->sib = *sib;
            this->modrm = *modrm;
        }
    private:
        Instruction(const Instruction& other);
        void operator=(const Instruction& other);
};