#include "Bios.hpp"

Bios *Bios::m_Bios = NULL;

Bios::Bios(void)
: m_Range(BIOS_MEM_START, BIOS_MEM_LENGTH)
{
}

Bios::~Bios(void)
{
    if (m_Data) {
        delete[] m_Data;
        m_Data = NULL;
    }
}

Bios *Bios::getInstance(void)
{
    if (!m_Bios) {
        m_Bios = new Bios();
    }
    return m_Bios;
}

bool Bios::contains(uint32_t addr)
{
    return m_Range.contains(addr);
}

uint32_t Bios::offset(uint32_t addr)
{
    return m_Range.offset(addr);
}

uint32_t Bios::load32(uint32_t offset)
{
    if (!m_Data) {
        fprintf(stderr, "Bios::load32: bios data is NULL!\n");
        return 0;
    }
    
    /* Get each 8 bits so we can reverse them for little endian */
    uint32_t byte0 = (uint32_t)m_Data[offset + 0];
    uint32_t byte1 = (uint32_t)m_Data[offset + 1];
    uint32_t byte2 = (uint32_t)m_Data[offset + 2];
    uint32_t byte3 = (uint32_t)m_Data[offset + 3];

    /* Return the bytes in reversed order */
    return byte0 | (byte1 << 8) | (byte2 << 16) | (byte3 << 24);
}

bool Bios::load(std::string fileName)
{
    std::ifstream file;
    file.open(fileName, std::ios::binary);

    if (!file.is_open()) {
        fprintf(stderr, "Bios::load: failed to open: %s\n", fileName.c_str());
        return false;
    }

    /* Get the size of the file */
    std::streampos fBegin, fEnd;
    fBegin = file.tellg();
    file.seekg(0, std::ios::end);
    fEnd = file.tellg();

    /* Make sure the file is the correct size */
    if ((fEnd-fBegin) != BIOS_SIZE) {
        fprintf(stderr, "Bios::load: incorrect file size!\n");
        file.close();
        return false;
    }
    
    /* Move the file back to the start */
    file.seekg(0, std::ios::beg);

    /* Init our member data and read in the file */
    if (m_Data) {
        delete[] m_Data;
    }
    m_Data = new uint8_t [BIOS_SIZE];
    file.read((char*)m_Data, BIOS_SIZE);

    /* BJ DEBUG */
    //for(int offset = 0xBFC00000 - BIOS_MEM_START; offset < BIOS_SIZE - 4; offset += 4)
    /*for(int offset = 0; offset < BIOS_SIZE - 4; offset += 4)
    {
        uint32_t instruction = load32(offset);
        uint32_t opcode = Instruction::function(instruction);
        uint32_t rs = Instruction::rs(instruction);
        uint32_t rt = Instruction::rt(instruction);
        uint32_t imm = Instruction::imm_se(instruction);
        uint32_t imm_jump= Instruction::imm_jump(instruction);
        printf("0x%08X    0x%08X    Op: 0x%02X    rs: 0x%02X    rt: 0x%02X    imm: 0x%08X    jmp_imm: 0x%08X\n",
             offset+BIOS_MEM_START, instruction, opcode, rs, rt, imm, imm_jump);
    }*/

    /*for (int i = 0; i < BIOS_SIZE; i++) {
        printf("%02c ", m_Data[i]);
        if (i != 0 && i % 8 == 0) {
            printf("\n");
        }
    }*/

    file.close();

    /* BJ DEBUG! */
    //exit(0);

    return true;
}



