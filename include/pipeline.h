#ifndef PIPELINE_H
#define PIPELINE_H

#include "queue.h"
#include "instructions.h"
#include "decoder.h"
#include "types.h"

void pipeline_cycle();
extern int sys_call;
extern int decode_stall;
extern int execute_stall;
extern int data_hazard;
extern int data_stall;
extern int stop;
extern struct EXEC EX; // This is now the only declaration of EX as a variable

#endif // PIPELINE_H
