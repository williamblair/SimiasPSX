#include "Ram.hpp"

Ram::Ram(void)
: data(2 * 1024 * 1024, 0xCA) // args - size, default value (garbage for now)
{
}

Ram::~Ram(void)
{
}

// fetch 32 bit little endian word at 'offset'
uint32_t Ram::load32(uint32_t offset)
{
    uint32_t b0 = (uint32_t)data[offset+0];
    uint32_t b1 = (uint32_t)data[offset+1];
    uint32_t b2 = (uint32_t)data[offset+2];
    uint32_t b3 = (uint32_t)data[offset+3];
    
    return (b0 | (b1 << 8) | (b2 << 16) | (b3 << 24));
}
    
// store 32 bit little endian word 'val' into 'offset'
void Ram::store32(uint32_t offset, uint32_t val)
{
    uint8_t b0 = (uint8_t)val;
    uint8_t b1 = (uint8_t)(val >> 8);
    uint8_t b2 = (uint8_t)(val >> 16);
    uint8_t b3 = (uint8_t)(val >> 24);
    
    data[offset+0] = b0;
    data[offset+1] = b1;
    data[offset+2] = b2;
    data[offset+3] = b3;
    
    return;
}

// check if the given address is within RAM
bool Ram::contains(uint32_t address)
{
    return ((address >= RAM_START) && (address < RAM_START + RAM_TOTAL));
}