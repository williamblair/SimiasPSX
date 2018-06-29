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
    
    /* Store a 32 bit value in memory */
    void store32(uint32_t addr, uint32_t value);
     
    /* Program Counter */
    uint32_t m_PC;

    /* General purpose registers */
    uint32_t m_Registers[32];

    /* Interconnect */
    Interconnect *m_Interconnect;
    
    /* The singleton instance */
    static Cpu *m_Cpu;
};

#endif

