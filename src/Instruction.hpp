#include <cstdint>

#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

/* Opcode decoding info can be found here: 
 * https://problemkaputt.de/psx-spx.htm#cpuopcodeencoding */
namespace Instruction {
    /* Bits 31-26 
     * function */
    uint32_t function(uint32_t instruction);
    
    /* bits 5-0 */
    uint32_t subfunction(uint32_t instruction);
    
    /* Bits 25-21 */
    uint32_t rs(uint32_t instruction);
    
    /* Bits 20-16 
     * register index */
    uint32_t rt(uint32_t instruction);
    
    /* Bits 15-11 */
    uint32_t rd(uint32_t instruction);
    
    /* Bits 10-6 
     * shift-imm value */
    uint32_t shift(uint32_t instruction);
    
    /* Bits 16-0 
     * immediate value */
    uint32_t imm(uint32_t instruction);
    
    /* Signed extension */
    uint32_t imm_se(uint32_t instruction);
}

#endif
