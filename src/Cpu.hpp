#include "Interconnect.hpp"
#include "Instruction.hpp"

#include <iostream>
#include <cstring>
#include <cstdint>
#include <climits>

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

class Cpu
{
public:

    /* Only public deconstructor */
    ~Cpu(void);

    /* Get the CPU instance */
    static Cpu *getInstance(void);    

    /* Set the interconnect */
    void setInterconnect(Interconnect *i);

    /* Get the instruction at PC, run it,
     * and increment the PC */
    void runNextInstruction(void);
    
    /* Load operations */
    void op_lw(uint32_t instruction);
    void op_lb(uint32_t instruction);
    void op_lui(uint32_t instruction);

    /* Store operations */
    void op_sb(uint32_t instruction);
    void op_sw (uint32_t instruction);
    void op_sh(uint32_t instruction);

    /* Addition operations */
    void op_addi(uint32_t instruction);
    void op_addiu(uint32_t instruction);
    void op_addu(uint32_t instruction);
    void op_add(uint32_t instruction);

    /* AND operations */
    void op_and(uint32_t instruction);
    void op_andi(uint32_t instruction);

    /* OR operations */
    void op_ori(uint32_t instruction);
    void op_or(uint32_t instruction);

    /* Jump operations */
    void op_j(uint32_t instruction);
    void op_jr(uint32_t instruction);
    void op_jal(uint32_t instruction);

    /* Branch operations */
    void op_beq(uint32_t instruction);
    void op_bne(uint32_t instruction);
    
    /* Cop0 operations */
    void op_cop0(uint32_t instruction);
    void op_mfc0(uint32_t instruction);
    void op_mtc0(uint32_t instruction);

    /* Other operations */
    void op_sll(uint32_t instruction);
    void op_sltu(uint32_t instruction);


private:
    
    /* Constructor */
    Cpu(void);

    /* Get the instruction from memory based on the PC as 8bit*/
    uint8_t load8(uint32_t addr);

    /* Get the instruction from memory based on the PC as 32bit */
    uint32_t load32(uint32_t addr);

    /* Parse the instruction and run it */
    void decodeAndExecute(uint32_t instruction);
    
    /* set a register value */
    void setRegister(uint32_t index, uint32_t value);
     
    /* Get a register value */
    uint32_t getRegister(uint32_t index);
    
    /* Store a 8 bit value in memory */
    void store8(uint32_t addr, uint8_t value);

    /* Store a 16 bit value in memory */
    void store16(uint32_t addr, uint16_t value);

    /* Store a 32 bit value in memory */
    void store32(uint32_t addr, uint32_t value);

    /* Branch by the given offset, adjusting for << 2 */
    void branch(uint32_t offset);

    /* Program Counter */
    uint32_t m_PC;
    
    /* The next loaded instruction, to simulate the branch
     * delay slot */
    uint32_t m_NextInstruction;

    /* General purpose registers */
    uint32_t m_Registers[32];

    /* Output registers (to be copied into m_Registers
     * each cycle) -> to handle load delay slots */
    uint32_t m_OutRegisters[32];

    /* The previous/queued Load into a register
     * First entry is the register, second entry is the
     * value to put in that register */
    uint32_t m_Load[2];

    /* Cop0 status register */
    uint32_t m_StatusRegister;

    /* Interconnect */
    Interconnect *m_Interconnect;
    
    /* The singleton instance */
    static Cpu *m_Cpu;
};

#endif

