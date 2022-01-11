#pragma once
#include "common.h"
class Instruction;

//デザインパターン：ファクトリメソッド
class InstructionFactory{
    public: 
        Instruction* CreateInstruction(const uint8_t op_code);
};  