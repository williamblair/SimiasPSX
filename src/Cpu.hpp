#include "Interconnect.hpp"

#include <iostream>
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
    
private:
    
    /* Constructor */
    Cpu(void);

    /* Get the instruction from memory based on the PC */
    uint32_t load32(uint32_t addr);

    /* Parse the instruction and run it */
    void decode_and_execute(uint32_t instruction);
     
    /* Program Counter */
    uint32_t m_PC;

    /* Interconnect */
    Interconnect *m_Interconnect;
    
    /* The singleton instance */
    static Cpu *m_Cpu;
};

#endif

