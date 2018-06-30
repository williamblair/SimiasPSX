#include "Cpu.hpp"

Cpu *Cpu::m_Cpu = NULL;

Cpu *Cpu::getInstance(void)
{
    if (!m_Cpu) {
        m_Cpu = new Cpu();
    }
    return m_Cpu;
}

Cpu::Cpu(void)
{
    /* Starting PC address of the PSX */
    m_PC = 0xBFC00000;
    
    /* Default the next instruction as a NOP */
    m_NextInstruction = 0;

    m_Interconnect = NULL;
    
    /* Initialize the register values */
    std::memset(m_Registers, 0, sizeof(m_Registers));
    
}

Cpu::~Cpu(void)
{
    
}

void Cpu::setInterconnect(Interconnect *i)
{
    m_Interconnect = i;
}

void Cpu::runNextInstruction(void)
{
    /* Get the previously loaded instruction */
    uint32_t instruction = m_NextInstruction;
    
    /* Get the instruction at the current program counter */
    m_NextInstruction = load32(m_PC);

    /* Increment the PC. In other languages we'd
     * have to make sure the PC moves back to 0 on 
     * overflow but C does that for us :) */
    m_PC += 4;

    /* Run the previously loaded */
    // DEBUG
    //printf("PC: 0x%08X    Instruction: 0x%08X\n", m_PC-8, instruction);
    //getchar();
    decodeAndExecute(instruction);
}

uint32_t Cpu::load32(uint32_t addr)
{
    return m_Interconnect->load32(addr);
}

void Cpu::store32(uint32_t addr, uint32_t value)
{
    m_Interconnect->store32(addr, value);
}

void Cpu::decodeAndExecute(uint32_t instruction)
{
    uint32_t function = Instruction::function(instruction);
    switch(function)
    {
        /* For Secondary opcodes */
        case 0b000000:
            switch(Instruction::subfunction(instruction))
            {
                case 0b000000: op_sll(instruction); break;
                case 0b100101: op_or(instruction);  break;
                
                default: quitWithInstruction("Cpu::decode_and_execute: unandled subfunction",
                            instruction);
            }
            break;
        
        case 0b001111: op_lui(instruction);   break;
        case 0b001101: op_ori(instruction);   break;
        case 0b101011: op_sw(instruction);    break;
        case 0b001001: op_addiu(instruction); break;
        case 0b000010: op_j(instruction);     break;
        
        default:       quitWithInstruction("Cpu::decode_and_execute: unhandled instruction",
                            instruction);
    }
}

void Cpu::setRegister(uint32_t index, uint32_t value)
{
    
    if (index < 32) {
        m_Registers[index] = value;
    }
    
    /* Can't set register 0 as its always 0 */
    m_Registers[0] = 0;
}

uint32_t Cpu::getRegister(uint32_t index)
{
    int regVal = 0;
    
    /* Unsigned so don't need to check < 0 */
    if (index < 32) {
        regVal = m_Registers[index];
    }
    
    else {
        /* Invalid register get */
        quitWithAddress("Cpu::getRegister: invalid register index", index);
    }
    
    return regVal;
}


/* Load Upper Immediate */
void Cpu::op_lui(uint32_t instruction)
{
    uint32_t i = Instruction::imm(instruction);
    uint32_t t = Instruction::rt(instruction);
    
    /* Lower 16 bits set to 0 */
    uint32_t value = i << 16;
    
    setRegister(t, value);
}

/* Bitwise Or Immediate */
void Cpu::op_ori(uint32_t instruction)
{
    uint32_t i = Instruction::imm(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t s = Instruction::rs(instruction);
    
    /* Bitwise ors the register and immediate value and stores result
     * in a register */
    uint32_t value = getRegister(s) | i;
    
    setRegister(t, value);
}

/* Store Word */
void Cpu::op_sw(uint32_t instruction)
{
    /* Store the contents of $t at the specified address plus an offset (i) 
     * The offset should be treated as a signed 16bit twos complement (so you
     * can have a negative offset) */
    uint32_t i = Instruction::imm_se(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t s = Instruction::rs(instruction);
    
    uint32_t addr = getRegister(s) + i;
    uint32_t value = getRegister(t);
    store32(addr, value);
}

/* Shift Left Logical */
void Cpu::op_sll(uint32_t instruction)
{
    uint32_t t = Instruction::rt(instruction);
    uint32_t d = Instruction::rd(instruction);
    uint32_t i = Instruction::shift(instruction);
    
    uint32_t value = getRegister(t) << i;
    setRegister(d, value);
}

/* Add immediate unsigned (no overflow) */
void Cpu::op_addiu(uint32_t instruction)
{
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t imm = Instruction::imm_se(instruction);
    
    /* Add the signed immediate value, ignoring overflow */
    uint32_t value = getRegister(s) + imm;
    setRegister(t, value);
}

/* Jump */
void Cpu::op_j(uint32_t instruction)
{
    uint32_t target = Instruction::imm_jump(instruction);
    
    /* The target given to use shifted right by two that
     * way the target can have 28 bits instead of 26, the first
     * two being used by the instruction identifier */
    m_PC = (m_PC & 0xF0000000) | (target << 2);
}

/* Or */
void Cpu::op_or(uint32_t instruction)
{
    uint32_t d = Instruction::rd(instruction);
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);

    /* ORs s and t and stores the result in d */
    uint32_t value = getRegister(s) | getRegister(t);
    setRegister(d, value);
}





