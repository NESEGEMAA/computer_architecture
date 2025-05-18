#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include "parser.h"
#include "queue.h"

// Define constants for Package 3
#define INSTRUCTION_SIZE 16
#define OPCODE_BITS 4
#define REG_BITS 6
#define IMMEDIATE_BITS 6

extern queue if_id_queue; // Instruction Fetch to Decode stage
extern queue id_ex_queue; // Decode to Execute stage

// // Structure to hold decoded instruction fields
// typedef struct {
//     int is_r_format;     // 1 for R-Format, 0 for I-Format
//     ID_EX id_ex;
// } DecodedInstruction;

// Function to check if instruction is R-Format
int isit_r_format(uint8_t opcode);
// Function to check if instruction needs sign extension for immediate
int needs_sign_extension(uint8_t opcode);

// Function to decode an instruction
extern void decode_stage();

#endif /* DECODER_H */