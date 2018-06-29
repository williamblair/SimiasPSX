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
    decode_and_execute(instruction);
}

uint32_t Cpu::load32(uint32_t addr)
{
    return m_Interconnect->load32(addr);
}

void Cpu::decode_and_execute(uint32_t instruction)
{
    quitWithMessage("Cpu::decode_and_execute: unhandled instruction",
        instruction);
}




