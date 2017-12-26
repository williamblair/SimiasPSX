/* Cpu class for psx emulator */

#include <cstdint>

#include "Interconnect.hpp"

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

enum CPU_REGISTER {
    REG_ZERO, // zero register,           $0
    REG_AT  , // assembler temporary,     $at 
    REG_V0  , // function return values,  $v0
    REG_V1  , //                          $v1
    REG_A0  , // function arguments       $a0
    REG_A1  , //                          $a1
    REG_A2  , //                          $a2
    REG_A3  , //                          $a3
    REG_T0  , // temporary registers      $t0
    REG_T1  , //                          $t1
    REG_T2  , //                          $t2
    REG_T3  , //                          $t3
    REG_T4  , //                          $t4
    REG_T5  , //                          $t5
    REG_T6  , //                          $t6
    REG_T7  , //                          $t7
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
    void     store32(uint32_t addr, uint32_t val); // used by sw op
    void     run_next_instruction(void); // executes code at pc and increments pc
    void     decode_and_execute(uint32_t instruction); // used by run_next_instruction
    
    // get and set values in the Cpu register
    uint32_t get_register(CPU_REGISTER r);
    bool     set_register(CPU_REGISTER r, uint32_t val);

private:

    // parses the instruction for the required bits for each piece of data
    // function - the instruction type (LUI, etc.) (bits 31:26)
    // s - which value to store into? (bits 25:21)
    // t - which register to use (bits 20:16)
    // imm - the immediate value to use (bits 16:0)
    uint32_t function(uint32_t instruction);
    uint32_t t(uint32_t instruction);
    uint32_t s(uint32_t instruction);
    uint32_t imm(uint32_t instruction);
    // returns the immediate value in 16 bits as a sign-extended 32 bit value
    uint32_t imm_se(uint32_t instruction);
    // returns the immmediate value in bits 25:0 - for jump instruction
    uint32_t imm_jump(uint32_t instruction);
    uint32_t d(uint32_t instruction);
    uint32_t subfunction(uint32_t instruction);
    uint32_t shift(uint32_t instruction);
    
    // execute a LUI operation
    void op_lui(uint32_t instruction);
    
    // execute a ORI operation
    void op_ori(uint32_t instruction);
    
    // execute a SW operation (store word)
    void op_sw(uint32_t instruction);
    
    // execute a SLL operation (shift left)
    void op_sll(uint32_t instruction);

	// execute an ADDIU operation (add immediate unsigned)
	void op_addiu(uint32_t instruction);
    
    // execute a J operation (jump - change the pc to a specified value)
    void op_j(uint32_t instruction);
    
    // execute an OR operation
    void op_or(uint32_t instruction);

    uint32_t registers[32]; // 32 bit pc -> 32 registers -> 5 bits for specifying register
    uint32_t pc; // program counter
    uint32_t next_instruction; // next instruction to be executed (branch delay slot)
    Interconnect interconnect; // maps between cpu and bios
};

#endif
