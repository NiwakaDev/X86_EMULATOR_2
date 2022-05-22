#pragma once
#include "common.h"
#include "ModRM.h"
#include "Sib.h"
#include "InstructionHelper.h"

class Sib;
class Cpu;
class Memory;

class Instruction{
    protected:
        std::unique_ptr<Object> obj;
        Sib sib;
        ModRM modrm;
        void ParseModRM(Cpu& cpu, Memory& memory);
        void ParseRegIdx(Cpu& cpu, Memory& memory);
        uint8_t GetRM8(Cpu& cpu, Memory& memory);
        uint16_t GetRM16(Cpu& cpu, Memory& memory);
        uint32_t GetRM32(Cpu& cpu, Memory& memory);
        uint16_t GetR16ForEffectiveAddr(Cpu& cpu, Memory& memory);
        uint32_t GetEffectiveAddr(Cpu& cpu, Memory& memory);
        void SetRM8(Cpu& cpu, Memory& memory, const uint8_t data);
        void SetRM16(Cpu& cpu, Memory& memory, const uint16_t data);
        void SetRM32(Cpu& cpu, Memory& memory, const uint32_t data);
        std::string code_name;
        std::function<uint8_t(uint16_t addr)> io_in8;
        std::function<void(uint16_t addr, uint8_t data)> io_out8;
    public:
        void SetIoIn8(std::function<uint8_t(uint16_t addr)> io_in8){
            this->io_in8 = io_in8;
        }
        void SetIoOut8(std::function<void(uint16_t addr, uint8_t data)> io_out8){
            this->io_out8 = io_out8;
        }
        virtual void Run(Cpu& cpu, Memory& memory) = 0;
        Instruction(std::string code_name);
        virtual ~Instruction();
        const std::string& GetInstructionName() const{
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