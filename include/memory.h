#ifndef MEMORY_H
#define MEMORY_H

#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

// Memory and Register File Declarations
#define REG_COUNT 64
#define DATA_MEMORY_SIZE 2048
#define INSTR_MEMORY_SIZE 1024

// Memory arrays
extern data_word_t register_file[REG_COUNT];
extern data_word_t data_memory[DATA_MEMORY_SIZE];
extern instruction_word_t instr_memory[INSTR_MEMORY_SIZE];

// Function declarations
void init_memory();
void init_instr_memory();
void init_data_memory();
void init_register_file();
instruction_word_t read_instruction(uint16_t address);
data_word_t read_data(uint16_t address);
void write_data(uint16_t address, data_word_t value);
data_word_t read_register(uint8_t reg_num);
void write_register(uint8_t reg_num, data_word_t value);

#endif // MEMORY_H