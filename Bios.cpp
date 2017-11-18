/* Bios class for psx emulator */

#include "Bios.hpp"

Bios::Bios(const std::string &fPath)
{
    // open the bios file
    std::ifstream biosFile(fPath.c_str(), std::ios::binary);
    if(!biosFile.is_open()){
        std::cerr << "Bios::Bios: Failed to open " << fPath << std::endl;
        exit(-1);
    }
    
    // read the data
    data = std::vector<uint8_t>((std::istream_iterator<uint8_t>(biosFile)),
                                 std::istream_iterator<uint8_t>());
                                 
    // make sure the size seems correct
    if(data.size() != BIOS_SIZE)
    {
        std::cerr << "Bios::Bios: Invalid Bios Size: " << data.size() << std::endl;
    }
}

uint32_t Bios::load32(uint32_t offset)
{
    uint32_t b0 = (uint32_t)data[offset + 0];
    uint32_t b1 = (uint32_t)data[offset + 1];
    uint32_t b2 = (uint32_t)data[offset + 2];
    uint32_t b3 = (uint32_t)data[offset + 3];
    
    return (b0 | (b1 << 8) | (b2 << 16) | (b3 << 24));
}

// check if a sent address is valid within the bios range
bool Bios::contains(uint32_t addr)
{
    return ((addr >= BIOS_START) && (addr < BIOS_START + BIOS_SIZE)); 
}
