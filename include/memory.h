#ifndef MEMORY_H
#define MEMORY_H
#include "globals.h"
#include <stdio.h>  // For fprintf
#include <stdlib.h> // For exit

// Memory and Register File Declarations
#define REG_COUNT 64              // 16 registers (R0-R63)
#define DATA_MEMORY_SIZE 2048      // 2048 bytes of data memory
#define INSTR_MEMORY_SIZE 1024     // 1024 instructions

// Memory arrays
extern data_word_t register_file[REG_COUNT];
extern data_word_t data_memory[DATA_MEMORY_SIZE];
extern instruction_word_t instr_memory[INSTR_MEMORY_SIZE];

// Pipelining intermittent variables
typedef struct {
    instruction_word_t instr; // 16-bit instruction
    uint16_t pc;   // PC at fetch time
} IF_ID;

typedef struct {
    instruction_word_t instr; // 16-bit instruction
    instruction_word_t pc;   // PC at fetch time
    uint8_t opcode;          // Decoded opcode (0–11)
    uint8_t r1, r2;          // Register indices
    int8_t imm;              // Immediate value (signed)
} ID_EX;

extern IF_ID if_id; // Instruction Fetch to Decode stage
extern ID_EX id_ex; // Decode to Execute stage

// Function declarations
void init_memory();
instruction_word_t read_instruction(uint16_t address);
data_word_t read_data(uint16_t address);
void write_data(uint16_t address, data_word_t value);
data_word_t read_register(uint8_t reg_num);
void write_register(uint8_t reg_num, data_word_t value);

#endif // MEMORY_H