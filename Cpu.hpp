/* Cpu class for psx emulator */

#include <cstdint>

#include "Interconnect.hpp"

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

enum CPU_REGISTER {
    REG_ZERO, // zero register
    REG_AT  , // assembler temporary
    REG_V0  , // function return values
    REG_V1  , 
    REG_A0  , // function arguments
    REG_A1  ,
    REG_A2  ,
    REG_A3  ,
    REG_T0  , // temporary registers
    REG_T1  ,
    REG_T2  ,
    REG_T3  ,
    REG_T4  ,
    REG_T5  ,
    REG_T6  ,
    REG_T7  ,
    REG_S0  , // saved registers
    REG_S1  ,
    REG_S2  ,
    REG_S3  ,
    REG_S4  ,
    REG_S5  ,
    REG_S6  ,
    REG_S7  ,
    REG_T8  , // temporary registers
    REG_T9  ,
    REG_K0  , // kernel reserved registers
    REG_K1  ,
    REG_GP  , // global pointer
    REG_SP  , // stack pointer
    REG_FP  , // frame pointer
    REG_RA    // function return address
};

class Cpu
{
public:
    Cpu(void){};
    Cpu(Interconnect &interconnect); // constructor

    uint32_t load32(const uint32_t address); // used by run_next_instruction
    void run_next_instruction(void); // executes code at pc and increments pc
    void decode_and_execute(uint32_t instruction); // used by run_next_instruction
    
    // get and set values in the Cpu register
    uint32_t get_register(CPU_REGISTER r);
    bool     set_register(CPU_REGISTER r, uint32_t val);

private:

    // parses the instruction for the required bits for each piece of data
    // function - the instruction type (LUI, etc.) (bits 31:26)
    // t - which register to use (bits 20:16)
    // imm - the immediate value to use (bits 16:0)
    uint32_t function(uint32_t instruction);
    uint32_t t(uint32_t instruction);
    uint32_t imm(uint32_t instruction);
    
    // execute a LUI operation
    void op_lui(uint32_t instruction);

    uint32_t registers[32]; // 32 bit pc -> 32 registers -> 5 bits for specifying register
    uint32_t pc; // program counter
    Interconnect interconnect; // maps between cpu and bios
};

#endif
