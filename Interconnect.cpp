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
        if(address % 4 != 0) {
            char e[100];
            sprintf(e, "Interconnect::load32: Unaligned fetch32 at address 0x%X", address);
            throw e;
        }
        else if(!bios.contains(address)){
            char e[100];
            sprintf(e, "Interconnect::load32: invalid fetch32 at address 0x%X", address);
            throw e;
        }
        // return the offset within the start of the bios
        instruction = bios.load32(address - BIOS_START);
        
    }
    catch(const char* exception)
    {
        std::cerr << "Exception: " << exception << std::endl;
        exit(-1);
    }

    return instruction;
}

void Interconnect::store32(uint32_t addr, uint32_t val)
{
    char e[100]; // exception error message
    
    try
    {
        // make sure the address is 32 bit aligned
        if(addr % 4 != 0) {
            
            sprintf(e, "Interconnect::store32: Unaligned store32 at address 0x%X", addr);
            throw e;
        }
        
        // make sure the setting address is in the proper range
        if(addr < MEM_CONTROL || addr >= MEM_CONTROL + MEM_CONTROL_SIZE) {
            sprintf(e, "Interconnect::store32: Out of range address 0x%X", addr);
            throw e;
        }
        
        // calculate the offset from the start of the MEM_CONTROL area
        uint32_t offset = addr - MEM_CONTROL;
        std::cout << "Interconnect::store32 Offset: " << offset << std::endl;
        
        switch(offset)
        {
            case 0:
                if(val != 0x1f000000){
                    sprintf(e, "Interconnect::store32: Bad expansion 1 base address: 0x%X", addr);
                    throw e;
                }
                break;
            case 4:
                if(val != 0x1f802000){
                    sprintf(e, "Interconnect::store32: Bad expansion 2 base address: 0x%X", addr);
                    throw e;
                }
                break;
            default:
                sprintf(e, "Interconnect::store32: Unhandled write to MEM_CONTROL, address 0x%X", addr);
                throw e;
        }
        
        // There will be more here later, it seems that for now we are fine...
        
        sprintf(e, "Interconnect::store32: Unhandled address 0x%X", addr);
        throw e;
    }
    catch(const char* s)
    {
        std::cerr << "Exception: " << s << std::endl;
        exit(-1);
    }
}
