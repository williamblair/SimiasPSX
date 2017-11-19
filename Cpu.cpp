/* Cpu class for psx emulator */

#include "Cpu.hpp"
#include "Bios.hpp"

#include <bitset>

/************************************************/
/**              CONSTRUCTOR                   **/
/************************************************/

Cpu::Cpu(Interconnect &interconnect)
{
    // the default program counter value in PSX
    // the beginning of the BIOS
    this->pc = BIOS_START;
    
    // internal interconnect object
    this->interconnect = interconnect;
    
    // set the initial register values
    registers[0] = 0; // the first register equals the $zero register
    
    // don't know what the reset values for these
    // registers are, so we give them a dummy placeholder
    for(int i=1; i<32; i++){
        registers[i] = 0xdeadbeef;
    }
}

/************************************************/
/**              GETTERS/SETTERS               **/
/************************************************/

bool Cpu::set_register(CPU_REGISTER r, uint32_t val)
{
    bool ret_val = true;
    
    if(r != REG_ZERO) {
        registers[r] = val;
    }
    else {
        std::cerr << "Cpu::set_register: cannot set REG_ZERO!\n";
        ret_val = false;
    }
    
    return ret_val;
}

uint32_t Cpu::get_register(CPU_REGISTER r)
{
    return registers[r];
}

/************************************************/
/**               OPERATIONS                   **/
/************************************************/

void Cpu::op_lui(uint32_t instruction)
{
    uint32_t i = imm(instruction);
    uint32_t reg = t(instruction);
    
    // low 16 bits set to 0
    uint32_t v = i << 16;
    
    set_register((CPU_REGISTER)reg, v);
    //std::cout << "Register " << reg << ": " << std::hex << "0x" << registers[reg] << std::endl;
    //exit(0);
}

// bitwise or of the immediate value and the given register
void Cpu::op_ori(uint32_t instruction)
{
    uint32_t i     = imm(instruction);
    uint32_t reg   = t(instruction);
    uint32_t store = s(instruction);
    
    uint32_t v = get_register((CPU_REGISTER)store) | i;
    
    set_register((CPU_REGISTER)reg, v);
    std::cout << "Register " << reg << ": " << std::hex << v << std::endl;
}

/************************************************/
/**            INSTRUCTION PARSING             **/
/************************************************/

// bits 31:26
uint32_t Cpu::function(uint32_t instruction)
{
    return instruction >> 26;
}

// bits 20:16
uint32_t Cpu::t(uint32_t instruction)
{
    // 0x1f -> 0b11111 (5 bits)
    return (instruction >> 16) & 0x1F;
}

// bits 25:21
uint32_t Cpu::s(uint32_t instruction)
{
    // 0x1f -> 0b11111 (5 bits)
    return (instruction >> 21) & 0x1F;
}

// bits 16:0
uint32_t Cpu::imm(uint32_t instruction)
{
    return instruction & 0xFFFF;
}

/************************************************/
/**            EXECUTION FUNCTIONS             **/
/************************************************/

uint32_t Cpu::load32(const uint32_t address)
{
    return interconnect.load32(address);
}

void Cpu::decode_and_execute(uint32_t instruction)
{
    try 
    {
        // we want bits 26-31 (farthest left/most significant),
        // so move them 26 bits to the right
        // 001111 -> LUI -> "Load Upper Immediate" -> Immediate -> 
        // the instruction is loaded straight away and not stored
        // somewhere else before execution -> Upper -> bits are stored
        // in the upper high 16 bits of the target register, lower
        // 16 bits are cleared (set to 0)
        
        // get the instruction type then decide what to do:
        uint32_t instruction_type = function(instruction);
        
        switch(instruction_type)
        {
            // checks the binary value
            case 0b001111:
                std::cout << "0b001111 (LUI) case!\n";
                op_lui(instruction);
                break;
            case 0b001101:
                std::cout << "0b001101 (ori) case!\n";
                op_ori(instruction);
                break;
            default:
                throw instruction;
            //    break;
        }
     
        //std::cout << "Instruction: " << std::bitset<32>(instruction) << std::endl;
        //std::cout << " Mix val     : " << std::bitset<32>(0xFFFF) << std::endl;
        //std::cout << "Immediate val: " << std::bitset<32>(immediate_val) << std::endl;
        
    }
    catch(uint32_t i)
    {
        std::cerr << "Cpu::decode_and_execute: Unhandled instruction: " << 
            std::hex << "0x" << i << std::endl;
        exit(-1);
    }
}

void Cpu::run_next_instruction(void)
{
    // get the code to run
    // requires pc but we don't have to send it as an arg
    uint32_t instruction = this->load32(pc);
    
    // get the next instruction
    // on overflow this should restart at 0, which c automatically does
    // i.e. 0xfffffffc + 4 => 0x00000000
    pc += 4;
    
    // run the instruction
    decode_and_execute(instruction);
}

