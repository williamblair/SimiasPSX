#include "Bios.hpp"
#include "Quit.hpp"

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

    /* Set our member pointers */
    void setBios(Bios *bios);

private:

    /* Constructor */
    Interconnect(void);

    /* Pointers to the Bios instance */
    Bios *m_Bios;

    /* Singelton instance */
    static Interconnect *m_Interconnect;
};

#endif

