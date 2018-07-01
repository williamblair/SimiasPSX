#include <cstdio>
#include <cstdint>

#ifndef RANGE_H_INCLUDED
#define RANGE_H_INCLUDED

class Range
{
public:
    
    /* Constructor */
    Range(uint32_t start, uint32_t length);

    /* Returns true if the addr is within the range */
    bool contains(uint32_t addr);

    /* Returns the offset of the addr with regards to the start */
    uint32_t offset(uint32_t addr);

private:
    uint32_t m_Start;
    uint32_t m_Length;
};

#endif


