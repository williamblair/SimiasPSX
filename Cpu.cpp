/* Cpu class for psx emulator */

#include "Cpu.hpp"
#include "Bios.hpp"

#include <cstring>
#include <bitset>
#include <climits> // to check of addition will result in overflow (ADDI)

/************************************************/
/**              CONSTRUCTOR                   **/
/************************************************/

Cpu::Cpu(Interconnect &interconnect)
{
    // the default program counter value in PSX
    // the beginning of the BIOS
    this->pc = BIOS_START;
    
    this->next_instruction = 0x0; // NOOP
    
    this->sr = 0;
    
    this->load[0] = this->load[1] = 0;
    
    // internal interconnect object
    this->interconnect = interconnect;
    
    // set the initial register values
    registers[0] = 0; // the first register equals the $zero register
    out_registers[0] = 0;
    
    // don't know what the reset values for these
    // registers are, so we give them a dummy placeholder
    for(int i=1; i<32; i++){
        registers[i] = 0xdeadbeef;
        out_registers[i] = 0xdeadbeef;
    }
}

/************************************************/
/**              GETTERS/SETTERS               **/
/************************************************/

bool Cpu::set_register(CPU_REGISTER r, uint32_t val)
{
    bool ret_val = true;
    
    if(r != REG_ZERO) {
        //registers[r] = val;
        out_registers[r] = val;
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
/**            INSTRUCTION PARSING             **/
/************************************************/

// bits 31:26
uint32_t Cpu::function(uint32_t instruction)
{
    return instruction >> 26;
}

// bits 25:21
uint32_t Cpu::s(uint32_t instruction)
{
    // 0x1f -> 0b11111 (5 bits)
    return (instruction >> 21) & 0x1F;
}

// bits 20:16
uint32_t Cpu::t(uint32_t instruction)
{
    // 0x1f -> 0b11111 (5 bits)
    return (instruction >> 16) & 0x1F;
}

// bits 16:0
uint32_t Cpu::imm(uint32_t instruction)
{
    return instruction & 0xFFFF;
}

// bits 16:0 but casted as a signed 32bit extended value first
uint32_t Cpu::imm_se(uint32_t instruction)
{
    int16_t v = (int16_t)(instruction & 0xffff);
    
    return (uint32_t)v;
}

// bits 25:0
uint32_t Cpu::imm_jump(uint32_t instruction)
{
    return instruction & 0x3FFFFFF;
}

// bits 15:11
uint32_t Cpu::d(uint32_t instruction)
{
    return (instruction >> 11) & 0x1F;
}

// bits 5:0
uint32_t Cpu::subfunction(uint32_t instruction)
{
    return instruction & 0x3F;
}

// get values to shift, which are stored in bits 10:6
uint32_t Cpu::shift(uint32_t instruction)
{
    return (instruction >> 6) & 0x1F;
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
}

// store 32 bits at a specific register into another register
void Cpu::op_sw(uint32_t instruction)
{
    // don't write if the cache is islocated
    if((this->sr & 0x10000) != 0) {
        std::cout << "Cpu::op_sw: Cache is isolated; ignoring write\n";
        return;
    }
    
    uint32_t i     = imm_se(instruction);
    uint32_t reg   = t(instruction);
    uint32_t store = s(instruction);
    
    uint32_t addr = get_register((CPU_REGISTER)store) + i;
    uint32_t v    = get_register((CPU_REGISTER)reg);
    
    store32(addr, v);
}

// shift left logical
// the preferred method to perfrom a noop is to shift left $zero
void Cpu::op_sll(uint32_t instruction)
{
    uint32_t i     = shift(instruction);
    uint32_t reg   = t(instruction);
    uint32_t store  = d(instruction);
    
    uint32_t val = get_register((CPU_REGISTER)reg) << i;
    
    set_register((CPU_REGISTER)store, val);
}

void Cpu::op_addiu(uint32_t instruction)
{
    uint32_t i     = imm_se(instruction);
    uint32_t reg   = t(instruction);
    //uint32_t store = s(instruction);
    
    // would use wrapping add here but c will wrap for us
    uint32_t val = get_register((CPU_REGISTER)reg) + i;
    
    set_register((CPU_REGISTER)reg, val);
}

void Cpu::op_addi(uint32_t instruction)
{
    int32_t i     = (int32_t)imm_se(instruction);
    uint32_t reg   = t(instruction);
    //uint32_t store = s(instruction);
    
    // get the value at the given register
    int32_t val = (int32_t)get_register((CPU_REGISTER)reg);
    
    // check if addition will result in overflow
    if(val > INT32_MAX - i) {
        std::cerr << "Cpu::op_addi: exception: addition will result in overflow!\n";
        std::cerr << "    Values: " << val << ", " << i << ", max is " << INT32_MAX
            << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // otherwise add the value
    val += i;
    
    set_register((CPU_REGISTER)reg, (uint32_t)val);
}

void Cpu::op_j(uint32_t instruction)
{
    uint32_t i = imm_jump(instruction);
    
    pc = (pc & 0xF0000000) | (i << 2);
    
    return;
}

void Cpu::op_or(uint32_t instruction)
{
    uint32_t reg = d(instruction);
    uint32_t val1 = s(instruction);
    uint32_t val2 = t(instruction);
    
    uint32_t result = get_register((CPU_REGISTER)val1) |
                      get_register((CPU_REGISTER)val2);
                      
    set_register((CPU_REGISTER)reg, result);
    
    return;
}

void Cpu::op_cop0(uint32_t instruction)
{
    // Simias has this as a different function (cop_opcode), but 
    // says it returns the same thing as s except instead of a cpu register
    // it's a u32 value
    uint32_t cop_opcode = s(instruction);
    switch(cop_opcode)
    {
        case 0b00100:
            std::cout << "op_cop0: MTC0 (move to coprocessor 0) case!\n";
            cop0_mtc0(instruction);
            break;
        
        default:
            std::cerr << "Cpu::op_cop0: unhandled cop0 instruction function: "
                << std::hex << cop_opcode << std::endl;
            exit(EXIT_FAILURE);
    }
    
    return;
}

void Cpu::op_bne(uint32_t instruction)
{
    uint32_t i     = imm_se(instruction);
    uint32_t store = s(instruction);
    uint32_t reg   = t(instruction);
    
    // if the values are not equal, branch with given offset i
    if(get_register((CPU_REGISTER)store) != get_register((CPU_REGISTER)reg)){
        this->branch(i);
    }
    
    return;
}

void Cpu::op_lw(uint32_t instruction)
{
    // check for isolated cache
    if((this->sr & 0x10000) != 0) {
        // cache is isolated, ignore load
        std::cout << "Cpu::op_lw: cache isolated, ignoring load\n";
        return;
    }
    
    uint32_t i   = imm_se(instruction);
    uint32_t reg1 = t(instruction);
    uint32_t reg2 = s(instruction);
    
    uint32_t addr = get_register((CPU_REGISTER)reg2) + i;
    
    uint32_t val = load32(addr);
    
    //set_register((CPU_REGISTER)reg1, val);
    this->load[0] = reg1; this->load[1] = val;
    
    return;
}

/************************************************/
/**             COP0 INSTRUCTIONS              **/
/************************************************/

void Cpu::cop0_mtc0(uint32_t instruction)
{
    uint32_t cpu_r = t(instruction);
    uint32_t cop_r = d(instruction);
    
    uint32_t v = get_register((CPU_REGISTER)cpu_r);
    
    // now write to the appropriate cop0 register
    switch(cop_r)
    {
        case 12:
            std::cout << "cop0_mtc0: register 12 (SR) write!\n";
            this->sr = v;
            break;
        default:
            std::cerr << "Cpu::cop0_mtc0: Unhandled cop0 register: "
                << std::hex << cop_r << std::endl;
            exit(EXIT_FAILURE);
    }
    
    return;
}

/************************************************/
/**            EXECUTION FUNCTIONS             **/
/************************************************/

// adjust the PC based on the given offset
void Cpu::branch(uint32_t offset)
{
    // offset immediates are always shifted 2 places
    // to the right since PC addresses have to be aligned
    // on 32 bits at all times
    offset = offset << 2;
    
    pc += offset;
    
    // need to compensate for the hardcoded 'pc += 4'
    // in run_next_instruction
    pc -= 4;
    
    return;
}

// load the 32 bit value from the given address
uint32_t Cpu::load32(const uint32_t address)
{
    return interconnect.load32(address);
}

// store val at the given address
void Cpu::store32(uint32_t addr, uint32_t val)
{
    // pass it along to the interconnect
    interconnect.store32(addr, val);
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
            case 0b000000:
                std::cout << "0b000000 (SLL) case!\n";
                switch(subfunction(instruction))
                {
                    case 0b000000:
                        op_sll(instruction);
                        break;
                    case 0b100101:
                        op_or(instruction);
                        break;
                    default:
                        std::cerr << "Cpu::decode_and_execute: NOOP (0b000000): unhandled subfunction\n";
                        throw subfunction(instruction);
                }
                break;
            case 0b010000:
                std::cout << "0b010000 (cop0) case!\n";
                op_cop0(instruction);
                break;
            case 0b001111:
                std::cout << "0b001111 (LUI) case!\n";
                op_lui(instruction);
                break;
            case 0b001101:
                std::cout << "0b001101 (ori) case!\n";
                op_ori(instruction);
                break;
            case 0b101011:
                std::cout << "0b101011 (sw) case!\n";
                op_sw(instruction);
                break;
            case 0b001000:
                std::cout << "0b001000 (addi) case!\n";
                op_addi(instruction);
                break;
            case 0b001001:
                std::cout << "0b001001 (addiu) case!\n";
                op_addiu(instruction);
                break;
            case 0b000010:
                std::cout << "0b000010 (J) case!\n";
                op_j(instruction);
                break;
            case 0b000101:
                std::cout << "0b000101 (BNE) case!\n";
                op_bne(instruction);
                break;
            case 0b100011:
                std::cout << "0b100011 (LW) case!\n";
                op_lw(instruction);
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
    //uint32_t instruction = this->load32(pc);
    
    // use previously loaded instruction
    uint32_t instruction = next_instruction;
    
    // get the next instruction at PC
    next_instruction = this->load32(pc);
    
    // execute a pending load (if any, otherwise loads $zero which
    // is a NOP)
    uint32_t reg = load[0]; uint32_t val = load[1];
    this->set_register((CPU_REGISTER)reg, val);
    
    // reset load to default to $zero (NOP)
    load[0] = load[1] = 0; 
    
    // get the next instruction
    // on overflow this should restart at 0, which c automatically does
    // i.e. 0xfffffffc + 4 => 0x00000000
    pc += 4;
    
    // run the instruction
    decode_and_execute(instruction);
    
    // copy the output registers as input for the next instruction
    std::memcpy(registers, out_registers, sizeof(uint32_t)*32);
}

