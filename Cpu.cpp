/* Cpu class for psx emulator */

#include "Cpu.hpp"

void Cpu::Cpu(void){}

void Cpu::run_next_instruction(void)
{
    // get the code to run
    // requires pc but we don't have to send it as an arg
    instruction = this->load32();
    
    // get the next instruction
    // on overflow this should restart at 0, which c automatically does
    // i.e. 0xfffffffc + 4 => 0x00000000
    this->pc += 4;
    
    // run the instruction
    this->decode_and_execute(instruction);
}
