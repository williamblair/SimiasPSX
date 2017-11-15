/* Interconnect class for psx emulator
 * maps between the cpu and bios */

#include "Bios.hpp"

#ifndef INTERCONNECT_H_INCLUDED
#define INTERCONNECT_H_INCLUDED

class Interconnect
{
public:
    // constructor
    // args: a Bios object
    Interconnect(Bios bios);

private:
    Bios bios;
};

#endif
