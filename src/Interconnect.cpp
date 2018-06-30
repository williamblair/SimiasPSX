#include "Interconnect.hpp"

Interconnect *Interconnect::m_Interconnect = NULL;

Interconnect *Interconnect::getInstance(void)
{
    if (!m_Interconnect) {
        m_Interconnect = new Interconnect();
    }
    return m_Interconnect;
}

Interconnect::Interconnect(void)
: MemControl   (0x1F801000, 36),
  RamSize      (0x1F801060, 4),
  CacheControl (0xFFFe0130, 4)
{
    m_Bios = NULL;
}

Interconnect::~Interconnect(void)
{
}

void Interconnect::setBios(Bios *bios)
{
    m_Bios = bios;
}

uint32_t Interconnect::load32(uint32_t addr)
{
    uint32_t instruction = 0;
     
    /* We only support aligned memory access just like the playstation! */
    if (addr % 4 != 0) {
        quitWithAddress("Interconnect::load32: unaligned address", addr);
    }
    
    if (m_Bios->contains(addr)) {
        uint32_t offset = m_Bios->offset(addr);
        instruction = m_Bios->load32(offset);
    }

    /* Unhandled address so far so halt */
    else {
        quitWithAddress("Interconnect::load32: unhandled address",
            addr);
    }

    return instruction;
}

void Interconnect::store32(uint32_t addr, uint32_t value)
{
    uint32_t offset;
    
    /* We only support aligned memory access just like the playstation! */
    if (addr % 4 != 0) {
        quitWithAddress("Interconnect::store32: unaligned address", addr);
    }
    
    /* Map to mem control */
    if (MemControl.contains(addr)) {
        
        /* Get the address offset */
        offset = MemControl.offset(addr);
        
        /* Make sure we're not trying to set expansion1/expansion2 addresses */
        if (MemControl.offset(addr) == 0 && value != 0x1F000000) {
            quitWithAddress("Interconnect::store32: bad expansion 1 addr", addr);
        }
        else if (MemControl.offset(addr) == 4 && value != 0x1F802000) {
            quitWithAddress("Interconnect::store32: bad expansion 2 addr", addr);
        }
        
        /* TODO - update */
        else {
            printf("Interconnect::store32: warning: unhandled write to mem control address: 0x%X\n", addr);
        }
    }
    
    /* Map to RAM Size */
    else if (RamSize.contains(addr)) {
        offset = RamSize.offset(addr);
        
        /* TODO - update */
        printf("Interconnect::store32: warning: unhandled write to RamSize address: 0x%X\n", addr);
    }
    
    /* Map to CACHE CONTROL */
    else if (CacheControl.contains(addr)) {
        offset = CacheControl.offset(addr);
        
        /* TODO - update */
        printf("Interconnect::store32: warning: unhandled write to CacheControl address: 0x%X\n", addr);
    }
    
    else {
        printf("Interconnect::store32: warning: unhandled write to address: 0x%X\n", addr);
    }
}
