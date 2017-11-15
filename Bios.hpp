/* Bios class for psx emulator */

#include <vector>
#include <iostream>
#include <fstream>

#ifndef BIOS_H_INCLUDED
#define BIOS_H_INCLUDED

// Bios images are always 512 kb in length
const int BIOS_FILE_SIZE = 512 * 1024;

class Bios
{
public:
    // constructor
    // args: file path of the bios file
    Bios(const string &fPath);
private:
    std::vector<uint8_t> data;
};

#endif
