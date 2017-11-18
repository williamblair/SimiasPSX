/* Bios class for psx emulator */

#include "Bios.hpp"
#include <iomanip>

Bios::Bios(const std::string &fPath)
 : data(NULL),
   data_length(0)
{
    FILE *fp = NULL;
    if(!(fp = fopen(fPath.c_str(), "rb")))
    {
        std::cerr << "Failed to open " << fPath << std::endl;
        exit(-1);
    }
    else
    {
        // get the size of the file
        fseek(fp, 0, SEEK_END);
        data_length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        // confirm the correct bios size
        if(data_length != BIOS_SIZE)
        {
            std::cerr << "Bios::Bios: invalid bios size: " << data_length;
            if(fp) fclose(fp);
            exit(-1);
        }
        
        // allocate data memory
        data = new unsigned char[data_length];
        
        // copy in the data
        int numReads = fread(data, sizeof(unsigned char), data_length, fp);
        
        // validate size of data read in
        if(numReads != BIOS_SIZE)
        {
            std::cerr << "Bios::Bios: Failed to read data correctly: " << numReads << std::endl;
            if(fp) fclose(fp);
            if(data) delete [] data;
            exit(-1);
        }
        
        // debug print out
        for(int i=1; i<=data_length; i++){
            std::cout << std::hex << std::setw(2) << data[i-1] << " ";
            if(i%5 == 0) std::cout << std::endl;
        }
        
        if(fp) fclose(fp);
    }
}

Bios::~Bios(void)
{
    if(data) delete[] data;
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
