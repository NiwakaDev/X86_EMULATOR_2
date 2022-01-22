#pragma once
#include "common.h"
class Instruction;

namespace InstructionHelper{
    const int INSTRUCTION_SET_SMALL_SIZE = 8;
    const int INSTRUCTION_SIZE = 256;
    void ShowInstructionName(Instruction& instruction);
    class InstructionFactory{
        public: 
            std::unique_ptr<Instruction> CreateInstruction(const uint8_t op_code);
    };  
}