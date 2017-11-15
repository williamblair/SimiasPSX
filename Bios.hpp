/* Bios class for psx emulator */

#include <vector>

#ifndef BIOS_H_INCLUDED
#define BIOS_H_INCLUDED

class Bios
{
public:
    // constructor
    // args: file path of the bios file
    Bios(const string &fPath);
private:
    std::vector<uint64_t> data;
};

#endif
