#ifndef DECODER_H
#define DECODER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // For fixed-width integer types

// Type definitions for assembler
typedef uint8_t  data_word_t;     // 8-bit data word
typedef uint16_t instruction_t;    // 16-bit instruction word

// Memory and Register File Declarations
#define REG_COUNT 64              // 64 registers (R0-R63)
#define DATA_MEMORY_SIZE 2048      // 2048 bytes of data memory
#define INSTR_MEMORY_SIZE 1024     // 1024 instructions

// External declarations - will be initialized in main.c
extern instruction_t PC;            // Program counter
extern data_word_t SREG;            // Status register
extern data_word_t register_file[REG_COUNT];        // Register file (R0-R63)
extern data_word_t data_memory[DATA_MEMORY_SIZE];   // Data memory
extern instruction_t instr_memory[INSTR_MEMORY_SIZE]; // Instruction memory

#endif // DECODER_H