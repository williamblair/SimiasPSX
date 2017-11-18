/* Interconnect class for psx emulator
 * maps cpu to bios */

#include "Interconnect.hpp"

Interconnect::Interconnect(Bios &bios)
{
    this->bios = bios;
}

uint32_t Interconnect::load32(const uint32_t address)
{
    uint32_t instruction = 0; // the output instruction

    try
    {
        if(!bios.contains(address)){
            throw "Interconnect::load32: invalid fetch32 at address " 
                  + address;
        }
        // return the offset within the start of the bios
        instruction = bios.load32(address - BIOS_START);
    }
    catch(std::string exception)
    {
        std::cerr << "Exception: " << exception << std::endl;
        exit(-1);
    }

    return instruction;
}

