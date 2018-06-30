#include "Instruction.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <sstream>

#ifndef QUIT_H_INCLUDED
#define QUIT_H_INCLUDED

void quitWithAddress(std::string message, uint32_t addr);
void quitWithInstruction(std::string message, uint32_t instruction);

#endif


