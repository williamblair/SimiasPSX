/* Bios class for psx emulator */

#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>

#ifndef BIOS_H_INCLUDED
#define BIOS_H_INCLUDED

// Bios images are always 512 kb in length
const uint32_t BIOS_SIZE = 512 * 1024;

// start of the bios in program counter memory
const uint32_t BIOS_START = 0xbfc00000;

class Bios
{
public:
    // constructor
    Bios(void){};

    // constructor
    // args: file path of the bios file
    Bios(const std::string &fPath);
    
    // fetch the 32 bit little endian word at 'offset'
    // little endian meaning if we have word 0x12345678,
    // its stored as [0x78, 0x65, 0x43, 0x21]
    uint32_t load32(uint32_t offset);
    
    // check if a sent address is valid within the bios range
    bool contains(uint32_t addr);
    
private:
    std::vector<uint8_t> data;
    
};

#endif
