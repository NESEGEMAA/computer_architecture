#include "globals.h"

// Global variable definitions
instruction_word_t PC = 0;            // Initialize Program Counter to 0
data_word_t SREG = 0;                 // Initialize Status Register to 0

int main() {
    // TODO: Add program initialization code
    printf("Computer Architecture Simulator Starting...\n");
    init_memory(); // Initialize memory and registers


    // TODO: Parse the assembly program and load it into instruction memory
    // parse_program("program.asm"); // To be implemented in parser.c


    // Main simulation loop
    while (PC < 1024) { // Assuming 1024 is the size of instruction memory
        // Fetch instruction
        instruction_word_t instr = read_instruction(PC);


        // TODO: Decode and Execute instruction
        // decode_and_execute(instr); // To be implemented in decoder.c

        
        // PC increment
        PC += 1;
    }

    return 0;
}