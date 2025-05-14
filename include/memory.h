#ifndef MEMORY_H
#define MEMORY_H
#include "globals.h"

// Memory and Register File Declarations
#define REG_COUNT 64              // 16 registers (R0-R63)
#define DATA_MEMORY_SIZE 2048      // 2048 bytes of data memory
#define INSTR_MEMORY_SIZE 1024     // 1024 instructions

void init_memory(); // Function to initialize memory and registers
instruction_word_t read_instruction(uint16_t address);
data_word_t read_data(uint16_t address);
void write_data(uint16_t address, data_word_t value);
data_word_t read_register(uint8_t reg_num);
void write_register(uint8_t reg_num, data_word_t value);

#endif // MEMORY_H