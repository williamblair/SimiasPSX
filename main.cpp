#include <iostream>

#include "Bios.hpp"
#include "Cpu.hpp"
#include "Interconnect.hpp"

int main(int argc, char **argv)
{
    std::cout << "Hello World!\n";
    
    if(argc != 2) 
    {
        std::cout << "Usage: " << argv[0] << "[bios_file]\n";
        return 0;
    }

    Bios bios(argv[1]);
    
    Interconnect interconnect(bios);
    
    Cpu cpu(interconnect);
    
    while(true)
    {
        cpu.run_next_instruction();
    }
    
    return 0;
}
