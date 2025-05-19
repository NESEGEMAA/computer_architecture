#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "types.h"
#include "memory.h"
#include "pipeline.h"
#include "queue.h"

extern instruction_word_t PC; 
extern data_word_t SREG;

extern queue if_id_queue; // Instruction Fetch to Decode stage
extern queue id_ex_queue; // Decode to Execute stage

void _ADD();
void _SUB();
void _MUL();
void _MOVI();
void _BEQZ();
void _ANDI();
void _EOR();
void _BR();
void _SAL();
void _SAR();
void _LDR();
void _STR();

#endif // INSTRUCTIONS_H