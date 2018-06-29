#include <cstdio>
#include <cstdint>
#include <string>
#include <fstream>

#include "Range.hpp"

#ifndef BIOS_H_INCLUDED
#define BIOS_H_INCLUDED

/* The bios should be 512 KB */
#define BIOS_SIZE (512 * 1024)

/* Memory start and length in PSX memory */
#define BIOS_MEM_START  0xBFC00000
#define BIOS_MEM_LENGTH BIOS_SIZE

class Bios
{
public:

    /* Only have public deconstructor */
    ~Bios(void);

    /* Get the singleton instance */
    static Bios *getInstance(void);

    /* Load the bios file into memory */
    bool load(std::string fileName);

    /* Test if the given address is within bios mem */
    bool contains(uint32_t addr);

    /* Return the offset in the internal range */
    uint32_t offset(uint32_t addr);

    /* Convert the bios data at the given offset
     * to little endian and return it */
    uint32_t load32(uint32_t offset);

private:

    /* Constructor */
    Bios(void);

    /* Our loaded bios in memory */
    uint8_t *m_Data;

    /* The address range of the BIOS within PSX memory */
    Range m_Range;

    /* The singleton instance */
    static Bios *m_Bios;
};

#endif


