#include "Quit.hpp"

void quitWithMessage(std::string message, uint32_t addr)
{
    std::stringstream ss;
    ss << message << ": 0x";
    ss << std::hex << addr << "\n";
    printf(ss.str().c_str());
    
    exit(0);
}

