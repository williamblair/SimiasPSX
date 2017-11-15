/* Interconnect class for psx emulator
 * maps cpu to bios */

#include "Interconnect.hpp"

Interconnect::Interconnect(Bios bios)
{
    this->bios = bios;
}
