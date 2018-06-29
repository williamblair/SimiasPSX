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
    /* Get the instruction at the current program counter */
    uint32_t instruction = load32(m_PC);

    /* Increment the PC. In other languages we'd
     * have to make sure the PC moves back to 0 on 
     * overflow but C does that for us :) */
    m_PC += 4;

    /* Run the instruction */
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
        case 0b001111: op_lui(instruction); break;
        case 0b001101: op_ori(instruction); break;
        case 0b101011: op_sw(instruction);  break;
        
        default:       quitWithMessage("Cpu::decode_and_execute: unhandled instruction",
                            instruction);
    }
}

void Cpu::setRegister(uint32_t index, uint32_t value)
{
    /* Can't set register 0 as its always 0 */
    if (index > 0 && index < 32) {
        m_Registers[index] = value;
    }
    
    /* Give a warning otherwise */
    else {
        printf("Cpu::setRegister: warning: invalid register index: 0x%8X\n", value);
    }
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
        quitWithMessage("Cpu::getRegister: invalid register index", index);
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
    uint32_t i = Instruction::imm(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t s = Instruction::rs(instruction);
    
    /* Store the contents of $t at the specified address plus an offset (i) */
    uint32_t addr = getRegister(s) + i;
    uint32_t value = getRegister(t);
    store32(addr, value);
}