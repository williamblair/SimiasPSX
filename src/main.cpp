#include "Cpu.hpp"
#include "Bios.hpp"
#include "Interconnect.hpp"

#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    /* Make sure we got our args */
    if (argc != 2) {
        printf("Usage: %s <bios_file>\n", argv[0]);
        return 0;
    }

    /* Get each instance */
    static Cpu          *cpu          = Cpu::getInstance();    
    static Bios         *bios         = Bios::getInstance();
    static Interconnect *interconnect = Interconnect::getInstance();

    /* Open the bios */
    if (!(bios->load(std::string(argv[1])))) {
        return -1;
    }

    /* Connect the Bios and Cpu */
    interconnect->setBios(bios);
    interconnect->setCpu(cpu);

    return 0;
}

