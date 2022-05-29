#pragma once
#include "common.h"
class Instruction;

namespace InstructionHelper {
const int INSTRUCTION_SET_SMALL_SIZE = 8;
const int INSTRUCTION_SIZE = 256;
void ShowInstructionName(const Instruction& instruction);
}  // namespace InstructionHelper