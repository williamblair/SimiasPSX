#include <vector>
#include <iostream>

#ifndef RAM_H_INCLUDED
#define RAM_H_INCLUDED

// Also defined in Interconnect.hpp, kind of messy...
const uint32_t RAM_START = 0xA0000000;
const uint32_t RAM_TOTAL = 2 * 1024 * 1024;

class Ram
{
public:
    // constructor/deconstructor
    Ram(void);
    ~Ram(void);
    
    // fetch 32 bit little endian word at 'offset'
    uint32_t load32(uint32_t offset);
    
    // store 32 bit little endian word 'val' into 'offset'
    void store32(uint32_t offset, uint32_t val);
    
    // check if the given address is within RAM
    bool contains(uint32_t address);

private:
    std::vector<uint8_t> data;

};

#endif
