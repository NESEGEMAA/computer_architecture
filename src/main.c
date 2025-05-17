// #include "globals.h"
// #include "pipelining.h"
// #include <stdio.h>

// // Global variable definitions
// instruction_word_t PC = 0;            // Initialize Program Counter to 0
// data_word_t SREG = 0;                 // Initialize Status Register to 0

// int main() {
//     // TODO: Add program initialization code
//     printf("Computer Architecture Simulator Starting...\n");
//     //init_memory(); // Initialize memory and registers
//     init_pipeline(); // Initialize pipeline and memory

//     // TODO: Parse the assembly program and load it into instruction memory
//     // parse_program("program.asm"); // To be implemented in parser.c


//     // Main simulation loop
//     // while (PC < 1024) { // Assuming 1024 is the size of instruction memory
//     //     // Fetch instruction
//     //     instruction_word_t instr = read_instruction(PC);


//     //     // TODO: Decode and Execute instruction
//     //     // decode_and_execute(instr); // To be implemented in decoder.c

        
//     //     // PC increment
//     //     PC += 1;
//     // }
//     run_pipeline(); // Run the pipeline simulation
//     return 0;
// }
/**
 * main.c
 * Test program for the parser of Package 3: Double Big Harvard combo large arithmetic shifts
 */

#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <assembly_file>\n", argv[0]);
        return 1;
    }

    // Parse the assembly file
    int instr_count = 0;
    Instruction* instructions = parse_assembly_file(argv[1], &instr_count);
    
    // Print the parsed instructions
    printf("Successfully parsed %d instructions:\n", instr_count);
    printf("-------------------------------------------\n");
    
    for (int i = 0; i < instr_count; i++) {
        printf("Instruction %d:\n", i);
        print_instruction(&instructions[i]);
        
        // Print binary representation in bits
        printf("Binary representation: ");
        for (int j = 15; j >= 0; j--) {
            printf("%d", (instructions[i].binary >> j) & 1);
            if (j % 4 == 0 && j > 0) printf(" ");
        }
        printf("\n\n");
    }
    
    // Print instruction memory content
    printf("Instruction Memory Content:\n");
    printf("-------------------------------------------\n");
    
    for (int i = 0; i < instr_count; i++) {
        printf("Address %03d: 0x%04X\n", i, instructions[i].binary);
    }
    
    // Free memory
    free_instructions(instructions);
    
    return 0;
}