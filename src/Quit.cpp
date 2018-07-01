#include "Quit.hpp"

void quitWithAddress(std::string message, uint32_t addr)
{
    std::stringstream ss;
    ss << message << ": 0x";
    ss << std::hex << addr << "\n";
    printf(ss.str().c_str());
    
    exit(0);
}

void quitWithInstruction(std::string message, uint32_t instruction)
{
    std::stringstream ss;
    ss << message << ": 0x";
    ss << std::hex << instruction << "\n";
    ss << "Function: 0x" << std::hex << Instruction::function(instruction);
    ss << "\n";
    
    printf(ss.str().c_str());
    
    exit(0);
}