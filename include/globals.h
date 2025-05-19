#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "queue.h"

// Global variable declarations
extern instruction_word_t PC;          // Program Counter
extern data_word_t SREG;         // Status Register

extern queue if_id_queue; // Instruction Fetch to Decode stage
extern queue id_ex_queue; // Decode to Execute stage

extern int sys_call; // Flag to indicate end of execution

#endif // GLOBALS_H