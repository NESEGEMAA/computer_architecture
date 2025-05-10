#include "globals.h"

// Global variable definitions
instruction_t PC = 0;            // Initialize Program Counter to 0
data_word_t SREG = 0;           // Initialize Status Register to 0

int main() {
    // TODO: Add program initialization code
    printf("Computer Architecture Simulator Starting...\n");
    init_memory(); // Initialize memory and registers

    // Main simulation loop
    while (PC < 1024) { // Assuming 1024 is the size of instruction memory
        // TODO: Add instruction fetch and execute cycle
        // TODO: Add PC increment logic
    }

    return 0;
}