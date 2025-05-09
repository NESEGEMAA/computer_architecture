#include "globals.h"

// Global variable definitions
instruction_t PC = 0;            // Initialize Program Counter to 0
data_word_t SREG = 0;           // Initialize Status Register to 0
data_word_t register_file[REG_COUNT] = {0};        // Initialize all registers to 0
data_word_t data_memory[DATA_MEMORY_SIZE] = {0};   // Initialize data memory to 0
instruction_t instr_memory[INSTR_MEMORY_SIZE] = {0}; // Initialize instruction memory to 0

int main() {
    // TODO: Add program initialization code
    printf("Computer Architecture Simulator Starting...\n");

    // Main simulation loop
    while (PC < INSTR_MEMORY_SIZE) {
        // TODO: Add instruction fetch and execute cycle
        // TODO: Add PC increment logic
    }

    return 0;
}