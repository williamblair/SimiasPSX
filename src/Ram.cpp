#include "Ram.hpp"

Ram *Ram::m_Ram = NULL;

Ram::Ram(void)
: m_Range(RAM_MEM_START, RAM_MEM_LENGTH)
{
    /* Init the RAM memory with garbage values */
    std::memset(m_Data, 0xCA, MEM_RAM_SIZE);
}

Ram::~Ram(void)
{
}

Ram *Ram::getInstance(void)
{
    if (!m_Ram) {
        m_Ram = new Ram();
    }
    return m_Ram;
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

    /* Get each 8 bits so we can reverse them for little endian */
    uint32_t byte0 = (uint32_t)m_Data[offset + 0];
    uint32_t byte1 = (uint32_t)m_Data[offset + 1];
    uint32_t byte2 = (uint32_t)m_Data[offset + 2];
    uint32_t byte3 = (uint32_t)m_Data[offset + 3];

    /* Return the bytes in reversed order */
    return byte0 | (byte1 << 8) | (byte2 << 16) | (byte3 << 24);
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
};

