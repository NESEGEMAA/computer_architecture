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

// Function to check if instruction is R-Format
int isit_r_format(uint8_t opcode);
// Function to check if instruction needs sign extension for immediate
int needs_sign_extension(uint8_t opcode);

// Function to decode an instruction
extern void decode_stage();

// Function to get opcode mnemonic string
const char *get_opcode_mnemonic(uint8_t opcode);

// Function to print decoded instruction in human-readable format
void print_decoded_instruction(uint8_t opcode, uint8_t r1, uint8_t r2, int8_t immediate, int is_r_format);

#endif /* DECODER_H */