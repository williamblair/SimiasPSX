#include "Interconnect.hpp"
#include "Instruction.hpp"

#include <iostream>
#include <cstring>
#include <cstdint>

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
    
    /* Operations */
    void op_lui(uint32_t instruction);
    void op_ori(uint32_t instruction);
    void op_sw (uint32_t instruction);
    void op_sll(uint32_t instruction);
    void op_addi(uint32_t instruction);
    void op_addiu(uint32_t instruction);
    void op_j(uint32_t instruction);
    void op_bne(uint32_t instruction);
    
    /* Coprocessor operations */
    void op_cop0(uint32_t instruction);
    void op_mtc0(uint32_t instruction);
    
private:
    
    /* Constructor */
    Cpu(void);

    /* Get the instruction from memory based on the PC */
    uint32_t load32(uint32_t addr);

    /* Parse the instruction and run it */
    void decodeAndExecute(uint32_t instruction);
    
    /* set a register value */
    void setRegister(uint32_t index, uint32_t value);
     
    /* Get a register value */
    uint32_t getRegister(uint32_t index);
    
    /* Set a cop0 register */
    void setCop0Register(uint32_t index, uint32_t value);
    
    /* Get a cop0 register */
    uint32_t getCop0Register(uint32_t index);
    
    /* Store a 32 bit value in memory */
    void store32(uint32_t addr, uint32_t value);

    /* Used by jump ops (like j, bne, etc.) */
    void branch(uint32_t offset);
     
    /* Program Counter */
    uint32_t m_PC;
    
    /* The next loaded instruction, to simulate the branch
     * delay slot */
    uint32_t m_NextInstruction;

    /* General purpose registers */
    uint32_t m_Registers[32];
    
    /* Coprocessor 0 registers 
     * Defined here: https://wiki.osdev.org/MIPS_Overview#Coprocessor_0_Registers 
     * Most of these aren't actually used but its easier to be able to access
     * the register numbers without any mapping to their index */
    uint32_t m_Cop0Registers[15];
    
    /* Interconnect */
    Interconnect *m_Interconnect;
    
    /* The singleton instance */
    static Cpu *m_Cpu;
};

#endif

