#include "Interconnect.hpp"

Interconnect *Interconnect::m_Interconnect = NULL;

Interconnect *Interconnect::getInstance(void)
{
    if (!m_Interconnect) {
        m_Interconnect = new Interconnect();
    }
    return m_Interconnect;
}

Interconnect::Interconnect(void)
{
    m_Bios = NULL;
}

Interconnect::~Interconnect(void)
{
}

void Interconnect::setBios(Bios *bios)
{
    m_Bios = bios;
}

uint32_t Interconnect::load32(uint32_t addr)
{
    uint32_t instruction = 0;

    if (m_Bios->contains(addr)) {
        uint32_t offset = m_Bios->offset(addr);
        instruction = m_Bios->load32(offset);
    }

    /* Unhandled address so far so halt */
    else {
        quitWithMessage("Interconnect::load32: unhandled address",
            addr);
    }

    return instruction;
}


