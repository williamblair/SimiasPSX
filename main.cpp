#include <iostream>

#include "Bios.hpp"
#include "Cpu.hpp"
#include "Interconnect.hpp"

int main(int argc, char **argv)
{
    std::cout << "Hello World!\n";
    
    Bios bios("../../SCPH1001.BIN");
    
    Interconnect interconnect(bios);
    
    Cpu cpu(interconnect);
    
    while(true)
    {
        cpu.run_next_instruction();
    }
    
    return 0;
}
