/* Cpu class for psx emulator */

#include "Cpu.hpp"
#include "Bios.hpp"

Cpu::Cpu(Interconnect &interconnect)
{
    // the default program counter value in PSX
    // the beginning of the BIOS
    this->pc = BIOS_START;
    
    // internal interconnect object
    this->interconnect = interconnect;
}

uint32_t Cpu::load32(const uint32_t address)
{
    return interconnect.load32(address);
}

void Cpu::decode_and_execute(uint32_t instruction)
{
    try 
    {
        throw instruction;
    }
    catch(uint32_t i)
    {
        std::cerr << "Cpu::decode_and_execute: Unhandled instruction: " << 
            std::hex << i << std::endl;
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

