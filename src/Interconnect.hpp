#include "Bios.hpp"
#include "Cpu.hpp"

#include <cstdio>

#ifndef INTERCONNECT_H_INCLUDED
#define INTERCONNECT_H_INCLUDED

class Interconnect
{
public:
    
    /* Only public deconstructor */
    ~Interconnect(void);

    /* Get the singleton instance */
    static Interconnect *getInstance(void);

    /* Set our member pointers */
    void setBios(Bios *bios);
    void setCpu (Cpu *cpu);

private:

    /* Constructor */
    Interconnect(void);

    /* Pointers to the Cpu and Bios instances */
    Bios *m_Bios;
    Cpu  *m_Cpu;

    /* Singelton instance */
    static Interconnect *m_Interconnect;
};

#endif

