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
}

Cpu::~Cpu(void)
{
    
}

void Cpu::run_next_instruction(void)
{
    /* Get the instruction at the current program counter */
    uint32_t instruction = load32();

    /* Increment the PC. In other languages we'd
     * have to make sure the PC moves back to 0 on 
     * overflow but C does that for us :) */
    m_PC += 4;

    /* Run the instruction */
    decode_and_execute(instruction);
}

// TODO
uint32_t Cpu::load32(void)
{
    return 1;
}

// TODO
void Cpu::decode_and_execute(uint32_t instruction)
{
    return;
}




