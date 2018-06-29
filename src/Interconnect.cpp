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
    if (m_Bios->contains(addr)) {
        uint32_t offset = m_Bios->offset(addr);
        return m_Bios->load32(offset);
    }

    /* Unhandled address so far, throw it! */
    else {
        std::stringstream ss;
        ss << "Interconnect::load32: unhandled address: ";
        ss << std::hex << addr;
        throw ss.str();
    }
}


