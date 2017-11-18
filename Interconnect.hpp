/* Interconnect class for psx emulator
 * maps between the cpu and bios */

#include "Bios.hpp"

#ifndef INTERCONNECT_H_INCLUDED
#define INTERCONNECT_H_INCLUDED

class Interconnect
{
public:

    Interconnect(void){};

    // constructor
    // args: a Bios object
    Interconnect(Bios &bios);

    uint32_t load32(uint32_t address);

private:
    Bios bios;
};

#endif
