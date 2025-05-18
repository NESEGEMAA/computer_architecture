#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include "memory.h"
#include "parser.h"

// Define constants for Package 3
#define INSTRUCTION_SIZE 16
#define OPCODE_BITS 4
#define REG_BITS 6
#define IMMEDIATE_BITS 6

// Structure to hold decoded instruction fields
typedef struct {
    uint8_t opcode;      // 4-bit opcode
    uint8_t r1;          // Register R1 number (6 bits)
    uint8_t r2;          // Register R2 number (6 bits, R-Format only)
    int16_t immediate;   // 6-bit immediate (I-Format, signed), aligned with parser
    uint16_t pc;         // PC of this instruction
    int is_r_format;     // 1 for R-Format, 0 for I-Format
    uint16_t instruction; // 16-bit instruction
} DecodedInstruction;

// Function to check if instruction is R-Format
int is_r_format(uint8_t opcode);

// Function to check if instruction needs sign extension for immediate
int needs_sign_extension(uint8_t opcode);

// Function to decode an instruction
extern DecodedInstruction decode_stage(uint16_t instruction, uint16_t pc, int cycle);

#endif /* DECODER_H */