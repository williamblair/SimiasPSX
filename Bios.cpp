/* Bios class for psx emulator */

#include "Bios.hpp"

Bios::Bios(const std::string &fPath)
{
    // open the bios file
    std::ifstream biosFile(fPath.c_str(), std::ios::binary);
    
    // read the data
    data = std::vector<uint64_t>((std::ifstreambuf_iterator<uint64_t>(biosFile)),
                                 std::ifstreambuf_iterator<uint64_t>());
                                 
    // make sure the size seems correct
    if(data.size() != BIOS_FILE_SIZE)
    {
        std::cerr << "Bios::Bios: Invalid Bios Size: " << data.size() << std::endl;
    }
}
