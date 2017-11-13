/* Cpu class for psx emulator */

#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

class Cpu
{
public:
    Cpu(void); // constructor

    void run_next_instruction(void); // executes code at pc and increments pc

private:
    uint32_t pc; // program counter
};

#endif
