/* Cpu class for psx emulator */

#include <cstdint>

#include "Interconnect.hpp"

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

class Cpu
{
public:
    Cpu(void){};
    Cpu(Interconnect &interconnect); // constructor

    uint32_t load32(const uint32_t address); // used by run_next_instruction
    void run_next_instruction(void); // executes code at pc and increments pc
    void decode_and_execute(uint32_t instruction); // used by run_next_instruction

private:
    uint32_t pc; // program counter
    Interconnect interconnect; // maps between cpu and bios
};

#endif
