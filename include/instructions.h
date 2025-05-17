#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "globals.h"
#include "memory.h"

extern instruction_word_t PC; 
extern data_word_t SREG; 

void _ADD(uint8_t rd, uint8_t rs);
void _SUB(uint8_t rd, uint8_t rs);
void _MUL(uint8_t rd, uint8_t rs);
void _MOVI(uint8_t rd, int8_t immediate);
void _BEQZ(uint8_t rs, int8_t immediate);
void _ANDI(uint8_t rd, int8_t immediate);
void _EOR(uint8_t rd, uint8_t rs);
void _BR(uint8_t rd, uint8_t rs);
void _SAL(uint8_t rd, int8_t immediate);
void _SAR(uint8_t rd, int8_t immediate);
void _LDR(uint8_t rd, uint16_t address);
void _STR(uint8_t rd, uint16_t address);

#endif // INSTRUCTIONS_H