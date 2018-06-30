#include "Range.hpp"

#include <cstdio>
#include <cstring>

#ifndef RAM_H_INCLUDED
#define RAM_H_INCLUDED

/* Instead of RAM_SIZE as RAM_SIZE is a
 * register which is confusing */
#define MEM_RAM_SIZE (2*1024*1024)

class Ram
{
public:

    /* Only have one public deconstructor */
    ~Ram(void);

    /* Get the singleton instance */
    static Ram *getInstance(void);

    /* Test if the given address is within
     * RAM memory */
    bool contains(uint32_t addr);

    /* Return the offset within the RAM memory */
    uint32_t offset(uint32_t addr);

    /* Get 32bit little endian at offset */
    uint32_t load32(uint32_t offset);

    /* Set 32bit little endian at offset */
    void store32(uint32_t offset, uint32_t value);

private:

    /* Private constructor for singleton */
    Ram(void);

    /* Our RAM memory */
    uint8_t m_Data[MEM_RAM_SIZE];

    /* The Ram's memory range within the PSX */
    Range m_Range;

    /* The singleton instance */
    static Ram *m_Instance;
};

#endif

