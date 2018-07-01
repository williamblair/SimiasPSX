#include "Range.hpp"

/* Constructor */
Range::Range(uint32_t start, uint32_t length)
{
    m_Start = start;
    m_Length = length;
}

bool Range::contains(uint32_t addr)
{
    return (addr >= m_Start && addr <= m_Start + m_Length);
}

uint32_t Range::offset(uint32_t addr)
{
    return addr - m_Start;
}

