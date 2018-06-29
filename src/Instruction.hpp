#include <cstdint>

#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

/* Opcode decoding info can be found here: 
 * https://problemkaputt.de/psx-spx.htm#cpuopcodeencoding */
namespace Instruction {
    /* Bits 31-26 
     * function */
    uint32_t function(uint32_t instruction);
    
    /* Bits 25-21 */
    uint32_t rs(uint32_t instruction);
    
    /* Bits 20-16 
     * register index */
    uint32_t rt(uint32_t instruction);
    
    /* Bits 16-0 
     * immediate value */
    uint32_t imm(uint32_t instruction);
}

#endif
