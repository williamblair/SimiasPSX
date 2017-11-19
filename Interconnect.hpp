/* Interconnect class for psx emulator
 * maps between the cpu and bios */
 
#include <iostream>
#include <iomanip>
 
#include "Bios.hpp"

#ifndef INTERCONNECT_H_INCLUDED
#define INTERCONNECT_H_INCLUDED

const uint32_t MEM_CONTROL      = 0x1F801000;
const uint32_t MEM_CONTROL_SIZE = 36;

class Interconnect
{
public:

    Interconnect(void){};

    // constructor
    // args: a Bios object
    Interconnect(Bios &bios);

    uint32_t load32(uint32_t address);
    void     store32(uint32_t addr, uint32_t val);

private:
    Bios bios;
};

#endif
