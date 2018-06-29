#include "Instruction.hpp"

namespace Instruction {
    uint32_t function(uint32_t instruction)
    {
        return instruction >> 26;
    }
    
    uint32_t rs(uint32_t instruction)
    {
        return (instruction >> 21) & 0x1F;
    }
    
    uint32_t rt(uint32_t instruction)
    {
        return (instruction >> 16) & 0x1F;
    }
    
    uint32_t imm(uint32_t instruction)
    {
        return instruction & 0xFFFF;
    }
}
