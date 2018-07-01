#include "Bios.hpp"
#include "Quit.hpp"
#include "Ram.hpp"

#include <cstdio>
#include <sstream>

#ifndef INTERCONNECT_H_INCLUDED
#define INTERCONNECT_H_INCLUDED

/* Masks to strip the region bits of the
 * address - uses 3MSBs of each address
 * KSEG2 not touched since it doesn't
 * share anything with the other regions */
const uint32_t REGION_MASK[8] = {
    /* KUSEG: 2048MB */
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

    /* KSEG0: 512MB */
    0x7FFFFFFF,

    /* KSEG1: 512MB */
    0x1FFFFFFF,

    /* KSEG2: 1024MB */
    0xFFFFFFFF, 0xFFFFFFFF
};

class Interconnect
{
public:
    
    /* Only public deconstructor */
    ~Interconnect(void);

    /* Get the singleton instance */
    static Interconnect *getInstance(void);

    /* Map the given address to its location
     * e.g. Bios etc. */
    uint32_t load32(uint32_t addr);
    
    /* Map and store the given halfword into the given address */
    void store16(uint32_t addr, uint16_t value);

    /* Map and store the given value into the given address */
    void store32(uint32_t addr, uint32_t value);

    /* Set our member pointers */
    void setBios(Bios *bios);
    void setRam(Ram *ram);

private:

    /* Constructor */
    Interconnect(void);

    /* Mask a CPU address to remove the region bits */
    uint32_t maskRegion(uint32_t addr);

    /* Pointers to the Bios and Ram instance */
    Bios *m_Bios;
    Ram  *m_Ram;
    
    /* Memory control range */
    Range MemControl;
    
    /* RAM_SIZE memory range */
    Range RamSize;

    /* Cache Control memory Range */
    Range CacheControl;

    /* Sound Processing Unit mem range */
    Range SPU;

    /* Singelton instance */
    static Interconnect *m_Interconnect;
};

#endif

