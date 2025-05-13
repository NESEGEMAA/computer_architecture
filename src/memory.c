#include "memory.h"

data_word_t register_file[REG_COUNT];        // Register file (R0-R63)
data_word_t data_memory[DATA_MEMORY_SIZE];   // Data memory
instruction_word_t instr_memory[INSTR_MEMORY_SIZE]; // Instruction memory

// Function to initialize instruction memory
void init_instr_memory() {
    for (uint16_t i = 0; i < INSTR_MEMORY_SIZE; i++) {
        instr_memory[i] = 0; // Initialize all instructions to 0
    }
}

// Function to initialize data memory
void init_data_memory() {
    for (uint16_t i = 0; i < DATA_MEMORY_SIZE; i++) {
        data_memory[i] = 0; // Initialize all data memory to 0
    }
}

// Function to initialize register file
void init_register_file() {
    for (uint16_t i = 0; i < REG_COUNT; i++) {
        register_file[i] = 0; // Initialize all registers to 0
    }
}

void init_memory() {
    init_instr_memory(); // Initialize instruction memory
    init_data_memory();  // Initialize data memory
    init_register_file(); // Initialize register file
}

// Function to read an instruction from instruction memory
instruction_word_t read_instruction(uint16_t address) {
    if (address < INSTR_MEMORY_SIZE) {
        return instr_memory[address];
    } else {
        fprintf(stderr, "Error: Instruction memory read out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}

// Function to read data from data memory
data_word_t read_data(uint16_t address) {
    if (address < DATA_MEMORY_SIZE) {
        return data_memory[address];
    } else {
        fprintf(stderr, "Error: Data memory read out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}

// Function to write data to data memory
void write_data(uint16_t address, data_word_t value) {
    if (address < DATA_MEMORY_SIZE) {
        data_memory[address] = value;
    } else {
        fprintf(stderr, "Error: Data memory write out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}

// Function to read a register value
data_word_t read_register(uint8_t reg_num) {
    if (reg_num < REG_COUNT) {
        return register_file[reg_num];
    } else {
        fprintf(stderr, "Error: Register read out of bounds at register %u\n", reg_num);
        exit(EXIT_FAILURE);
    }
}

// Function to write a value to a register
void write_register(uint8_t reg_num, data_word_t value) {
    if (reg_num < REG_COUNT) {
        register_file[reg_num] = value;
    } else {
        fprintf(stderr, "Error: Register write out of bounds at register %u\n", reg_num);
        exit(EXIT_FAILURE);
    }
}