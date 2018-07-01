#include "Instruction.hpp"

namespace Instruction {
    uint32_t function(uint32_t instruction)
    {
        return instruction >> 26;
    }
    
    uint32_t subfunction(uint32_t instruction)
    {
        return instruction & 0x3F;
    }
    
    uint32_t cop_function(uint32_t instruction)
    {
        return (instruction >> 21) & 0x1F;
    }

    uint32_t rs(uint32_t instruction)
    {
        return (instruction >> 21) & 0x1F;
    }
    
    uint32_t rt(uint32_t instruction)
    {
        return (instruction >> 16) & 0x1F;
    }
    
    uint32_t rd(uint32_t instruction)
    {
        return (instruction >> 11) & 0x1F;
    }
    
    uint32_t shift(uint32_t instruction)
    {
        return (instruction >> 6) & 0x1F;
    }
    
    uint32_t imm(uint32_t instruction)
    {
        return instruction & 0xFFFF;
    }
    
    uint32_t imm_se(uint32_t instruction)
    {
        int16_t value = ((int16_t)instruction & 0xFFFF);
        return (uint32_t) value;
    }
    
    uint32_t imm_jump(uint32_t instruction)
    {
        return instruction & 0x3FFFFFF;
    }
}
