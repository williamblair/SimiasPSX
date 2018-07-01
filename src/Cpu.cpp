#include "Cpu.hpp"

Cpu *Cpu::m_Cpu = NULL;

Cpu *Cpu::getInstance(void)
{
    if (!m_Cpu) {
        m_Cpu = new Cpu();
    }
    return m_Cpu;
}

Cpu::Cpu(void)
{
    /* Starting PC address of the PSX */
    m_PC = 0xBFC00000;
    
    /* Default the next instruction as a NOP */
    m_NextInstruction = 0;

    m_StatusRegister = 0;

    m_Interconnect = NULL;
    
    /* Initialize the register values */
    std::memset(m_Registers, 0, sizeof(m_Registers));
    std::memset(m_OutRegisters, 0, sizeof(m_OutRegisters));
    
    m_Load[0] = m_Load[1] = 0;
}

Cpu::~Cpu(void)
{
    
}

void Cpu::setInterconnect(Interconnect *i)
{
    m_Interconnect = i;
}

void Cpu::runNextInstruction(void)
{
    /* Get the previously loaded instruction */
    uint32_t instruction = m_NextInstruction;
    
    /* Get the instruction at the current program counter */
    m_NextInstruction = load32(m_PC);

    /* Increment the PC. In other languages we'd
     * have to make sure the PC moves back to 0 on 
     * overflow but C does that for us :) */
    m_PC += 4;

    /* Set the queued (if any) load register value */
    setRegister(m_Load[0], m_Load[1]);

    /* Set the queued load to moving a 0 in 0,
     * so basically a noop */
    m_Load[0] = m_Load[1] = 0;

    /* Run the previously loaded */
    decodeAndExecute(instruction);

    /* Copy the output registers as input for the
     * next instruction */
    std::memcpy(m_Registers, m_OutRegisters, sizeof(m_Registers));
}

uint32_t Cpu::load32(uint32_t addr)
{
    return m_Interconnect->load32(addr);
}

void Cpu::store16(uint32_t addr, uint16_t value)
{
    m_Interconnect->store16(addr, value);
}

void Cpu::store32(uint32_t addr, uint32_t value)
{
    m_Interconnect->store32(addr, value);
}

void Cpu::decodeAndExecute(uint32_t instruction)
{
    uint32_t function = Instruction::function(instruction);
    switch(function)
    {
        /* For Secondary opcodes */
        case 0b000000:
            switch(Instruction::subfunction(instruction))
            {
                case 0b000000: op_sll(instruction); break;
                case 0b100101: op_or(instruction);  break;
                case 0b101011: op_sltu(instruction); break;
                case 0b100001: op_addu(instruction); break;
                
                default: quitWithInstruction("Cpu::decode_and_execute: unandled subfunction",
                            instruction);
            }
            break;
        
        case 0b001111: op_lui(instruction);   break;
        case 0b001101: op_ori(instruction);   break;
        case 0b101001: op_sh(instruction);    break;
        case 0b101011: op_sw(instruction);    break;
        case 0b001000: op_addi(instruction);  break;
        case 0b001001: op_addiu(instruction); break;
        case 0b000010: op_j(instruction);     break;
        case 0b000011: op_jal(instruction);   break;
        case 0b000101: op_bne(instruction);   break;
        case 0b100011: op_lw(instruction);    break;
        case 0b001100: op_andi(instruction);  break;
        

        case 0b010000: op_cop0(instruction);  break;

        default:       quitWithInstruction("Cpu::decode_and_execute: unhandled instruction",
                            instruction);
    }
}

void Cpu::setRegister(uint32_t index, uint32_t value)
{
#if 0
    if (index < 32) {
        m_Registers[index] = value;
    }
    
    /* Can't set register 0 as its always 0 */
    m_Registers[0] = 0;
#endif

    /* Queue a register load */
    if (index < 32) {
        m_OutRegisters[index] = value;
    }

    /* Invalid index! */
    else {
        quitWithAddress("Cpu::setRegister: invalid index", index);
    }

    /* Can't set register 0 as its always 0 */
    m_OutRegisters[0] = 0;
}

uint32_t Cpu::getRegister(uint32_t index)
{
    int regVal = 0;
    
    /* Unsigned so don't need to check < 0 */
    if (index < 32) {
        regVal = m_Registers[index];
    }
    
    else {
        /* Invalid register get */
        quitWithAddress("Cpu::getRegister: invalid register index", index);
    }
    
    return regVal;
}

void Cpu::branch(uint32_t offset)
{
    /* Multiply by 4 to make sure we're always 32bit aligned */
    offset <<= 2;

    m_PC += offset;

    /* adjust for the pc += 4 in runNextInstruction,
     * not combined with above to allow for overflow? (I think)  */
    m_PC -= 4;
}

/* Load Upper Immediate */
void Cpu::op_lui(uint32_t instruction)
{
    uint32_t i = Instruction::imm(instruction);
    uint32_t t = Instruction::rt(instruction);
    
    /* Lower 16 bits set to 0 */
    uint32_t value = i << 16;
    
    setRegister(t, value);
}

/* Bitwise Or Immediate */
void Cpu::op_ori(uint32_t instruction)
{
    uint32_t i = Instruction::imm(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t s = Instruction::rs(instruction);
    
    /* Bitwise ors the register and immediate value and stores result
     * in a register */
    uint32_t value = getRegister(s) | i;
    
    setRegister(t, value);
}

/* Store halfword */
void Cpu::op_sh(uint32_t instruction)
{
    /* Don't write if cache isolated */
    if (m_StatusRegister & 0x10000 != 0) {
        printf("Cpu::op_sh: cache isolated!\n");
        return;
    }

    uint32_t imm = Instruction::imm_se(instruction);
    uint32_t t   = Instruction::rt(instruction);
    uint32_t s   = Instruction::rs(instruction);

    /* Get the address to write to plus offset */
    uint32_t addr = getRegister(s) + imm;
    uint32_t value = getRegister(t);

    store16(addr, (uint16_t) value);
}

/* Store Word */
void Cpu::op_sw(uint32_t instruction)
{
    /* Don't write if the status register isolation bit is set */
    if (m_StatusRegister & 0x10000 != 0) {
        printf("Cpu::op_sw: cache isolated\n");
        return;
    }

    /* Store the contents of $t at the specified address plus an offset (i) 
     * The offset should be treated as a signed 16bit twos complement (so you
     * can have a negative offset) */
    uint32_t i = Instruction::imm_se(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t s = Instruction::rs(instruction);
    
    uint32_t addr = getRegister(s) + i;
    uint32_t value = getRegister(t);
    store32(addr, value);
}

/* Shift Left Logical */
void Cpu::op_sll(uint32_t instruction)
{
    uint32_t t = Instruction::rt(instruction);
    uint32_t d = Instruction::rd(instruction);
    uint32_t i = Instruction::shift(instruction);
    
    uint32_t value = getRegister(t) << i;
    setRegister(d, value);
}

/* Add immediate unsigned (WITH overflow exception) */
void Cpu::op_addi(uint32_t instruction)
{
    int32_t imm = Instruction::imm_se(instruction);
    uint32_t rt = Instruction::rt(instruction);
    uint32_t rs = Instruction::rs(instruction);

    /* Check if (as signed ints) the addition will
     * cause overflow, and if so create an exception */
    int32_t s = (int32_t) getRegister(rs);
    if (s > (INT_MAX - imm)) {
        quitWithInstruction("Cpu::op_addi: overflow", instruction);
    }

    setRegister(rt, s + imm);
}

/* Add immediate unsigned (no overflow) */
void Cpu::op_addiu(uint32_t instruction)
{
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t imm = Instruction::imm_se(instruction);
    
    /* Add the signed immediate value, ignoring overflow */
    uint32_t value = getRegister(s) + imm;
    setRegister(t, value);
}

/* Add unsigned (no overflow) */
void Cpu::op_addu(uint32_t instruction)
{
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t d = Instruction::rd(instruction);

    /* $d = $s + $t */
    uint32_t value = getRegister(s) + getRegister(t);
    setRegister(d, value);
}

/* Jump */
void Cpu::op_j(uint32_t instruction)
{
    uint32_t target = Instruction::imm_jump(instruction);
    
    /* The target given to use shifted right by two that
     * way the target can have 28 bits instead of 26, the first
     * two being used by the instruction identifier */
    m_PC = (m_PC & 0xF0000000) | (target << 2);
}

/* Jump and Link */
void Cpu::op_jal(uint32_t instruction)
{
    uint32_t target = Instruction::imm_jump(instruction);

    /* Jump to calculated address and store the return
     * address in $31 */
    setRegister(31, m_PC);
    m_PC = (m_PC & 0xF0000000) | (target << 2);
}

/* Or */
void Cpu::op_or(uint32_t instruction)
{
    uint32_t d = Instruction::rd(instruction);
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);

    /* ORs s and t and stores the result in d */
    uint32_t value = getRegister(s) | getRegister(t);
    setRegister(d, value);
}

/* Branch if Not Equal */
void Cpu::op_bne(uint32_t instruction)
{
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t imm = Instruction::imm_se(instruction);

    /* Branch if the two registers are not equal */
    uint32_t regs = getRegister(s);
    uint32_t regt = getRegister(t);
    if (regs != regt) {
        //printf("BNE: instruction: 0x%08X    s: 0x%X    t: 0x%X    offset: %d\n", instruction, s, t, (int32_t)imm);
        branch(imm);
    }
}

/* Load Word */
void Cpu::op_lw(uint32_t instruction)
{
    /* Ignore isolated cache */
    if (m_StatusRegister & 0x10000 != 0) {
        printf("Cpu::op_lw: ignoring isolated cache!\n");
        return;
    }

    uint32_t imm = Instruction::imm_se(instruction);
    uint32_t t   = Instruction::rt(instruction);
    uint32_t s   = Instruction::rs(instruction);

    /* Get the address plus the given offset */
    uint32_t addr = getRegister(s) + imm;

    /* Load the value at address and queue a register load
     * value in the load delay slot */
    uint32_t value = load32(addr);

    m_Load[0] = t; m_Load[1] = value;
}

/* Set register if Less Than Unsigned */
void Cpu::op_sltu(uint32_t instruction)
{
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t d = Instruction::rd(instruction);

    /* If $s < $t $d = 1, $d = 0 otherwise */
    uint32_t rs = getRegister(s);
    uint32_t rt = getRegister(t);

    /* 1 or 0 */
    setRegister(d, (uint32_t)(rs < rt));
}

/* Bitwise And Immediate */
void Cpu::op_andi(uint32_t instruction)
{
    uint32_t s = Instruction::rs(instruction);
    uint32_t t = Instruction::rt(instruction);
    uint32_t imm = Instruction::imm(instruction);

    /* $t = $s & imm */
    uint32_t value = getRegister(s) & imm;
    setRegister(t, value);
}

/* Cop0 subfunction parser */
void Cpu::op_cop0(uint32_t instruction)
{
    uint32_t func = Instruction::cop_function(instruction);

    switch (func)
    {
        case 0b00100: op_mtc0(instruction); break;

        default: quitWithInstruction("Cpu::op_cop0: unhandled cop0 function", instruction);
    }
}

/* Move To Coprocessor 0 */
void Cpu::op_mtc0(uint32_t instruction)
{
    uint32_t t = Instruction::rt(instruction);
    uint32_t d = Instruction::rd(instruction);

    /* Get the value at CPU register t and copy it
     * into COP0 register d */
    uint32_t value = getRegister(t);

    switch (d)
    {
        case 3:
        case 5:
        case 6:
        case 7:
        case 9:
        case 11:

            if (value != 0) {
                quitWithAddress("Cpu::op_mtc0: Unhandled write to breakpoint register",
                    value);
            }
            break;

        case 12: m_StatusRegister = value; break;

        case 13:

            if (value != 0) {
                quitWithAddress("Cpu::op_mtc0: Unhandled write to CAUSE register", value);
            }
            break;

        default: quitWithAddress("Cpu::op_mtc0: Uhandled cop0 register", d);
    }
}






