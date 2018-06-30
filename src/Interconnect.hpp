#include "Bios.hpp"
#include "Quit.hpp"
#include "Ram.hpp"

#include <cstdio>
#include <sstream>

#ifndef INTERCONNECT_H_INCLUDED
#define INTERCONNECT_H_INCLUDED

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
    
    /* Map and store the given value into the given address */
    void store32(uint32_t addr, uint32_t value);

    /* Set our member pointers */
    void setBios(Bios *bios);
    void setRam(Ram *ram);

private:

    /* Constructor */
    Interconnect(void);

    /* Pointers to the Bios and RAM instance */
    Bios *m_Bios;
    Ram  *m_Ram;
    
    /* Memory control range */
    Range MemControl;
    
    /* RAM_SIZE memory range */
    Range RamSize;
    
    /* CACHE_CONTROL memory range */
    Range CacheControl;

    /* Singelton instance */
    static Interconnect *m_Interconnect;
};

#endif

