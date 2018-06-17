#include <cstdio>
#include <cstdint>
#include <string>
#include <fstream>

#ifndef BIOS_H_INCLUDED
#define BIOS_H_INCLUDED

/* The bios should be 512 KB */
#define BIOS_SIZE (512 * 1024)

class Bios
{
public:

    /* Only have public deconstructor */
    ~Bios(void);

    /* Get the singleton instance */
    static Bios *getInstance(void);

    /* Load the bios file into memory */
    bool load(std::string fileName);

private:

    /* Constructor */
    Bios(void);

    /* Convert the bios data at the given offset
     * to little endian and return it */
    uint32_t load32(uint32_t offset);

    /* Our loaded bios in memory */
    uint8_t *m_Data;

    /* The singleton instance */
    static Bios *m_Bios;
};

#endif


