#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

#include "Instruction.hpp"
#include "Range.hpp"
#include "Quit.hpp"

#ifndef RAM_H_INCLUDED
#define RAM_H_INCLUDED

/* 2MB Ram, named MEM_RAM_SIZE because
 * RAM_SIZE is a register to avoiding confusion */
#define MEM_RAM_SIZE (2 * 1024 * 1024)

/* Memory start and length in PSX memory */
#define RAM_MEM_START  0xA0000000
#define RAM_MEM_LENGTH MEM_RAM_SIZE

class Ram
{
public:

    /* Only have public deconstructor */
    ~Ram(void);

    /* Get the singleton instance */
    static Ram *getInstance(void);

    /* Test if the given address is within Ram mem */
    bool contains(uint32_t addr);

    /* Return the offset in the internal range */
    uint32_t offset(uint32_t addr);

    /* Convert the Ram data at the given offset
     * to little endian and return it */
    uint32_t load32(uint32_t offset);

    /* Store the 32bit value into 4 8bit chunks
     * in our data */
    void store32(uint32_t offset, uint32_t value);

private:

    /* Constructor */
    Ram(void);

    /* Our loaded Ram in memory */
    uint8_t m_Data[MEM_RAM_SIZE];

    /* The address range of the Ram within PSX memory */
    Range m_Range;

    /* The singleton instance */
    static Ram *m_Ram;
};

#endif


