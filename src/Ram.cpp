#include "Ram.hpp"

Ram *Ram::m_Instance = NULL;

Ram::Ram(void)
: m_Range(0xA0000000, MEM_RAM_SIZE)
{
    /* Ram's contents are initially garbage */
    std::memset(m_Data, 0xca, MEM_RAM_SIZE);
}

Ram::~Ram(void)
{
}

Ram *Ram::getInstance(void)
{
    if (m_Instance == NULL) {
        m_Instance = new Ram();
    }

    return m_Instance;
}

bool Ram::contains(uint32_t addr)
{
    return m_Range.contains(addr);
}

uint32_t Ram::offset(uint32_t addr)
{
    return m_Range.offset(addr);
}

uint32_t Ram::load32(uint32_t offset)
{
    uint32_t b0 = (uint32_t) m_Data[offset + 0];
    uint32_t b1 = (uint32_t) m_Data[offset + 1];
    uint32_t b2 = (uint32_t) m_Data[offset + 2];
    uint32_t b3 = (uint32_t) m_Data[offset + 3];

    return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
}

void Ram::store32(uint32_t offset, uint32_t value)
{
    uint8_t b0 = (uint8_t) value;
    uint8_t b1 = (uint8_t) (value >> 8);
    uint8_t b2 = (uint8_t) (value >> 16);
    uint8_t b3 = (uint8_t) (value >> 24);

    m_Data[offset + 0] = b0;
    m_Data[offset + 1] = b1;
    m_Data[offset + 2] = b2;
    m_Data[offset + 3] = b3;
}
