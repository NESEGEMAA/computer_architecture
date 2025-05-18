#include "globals.h"
#include "pipelining.h"
#include "memory.h"
#include <stdio.h>
#include "../include/parser.h"

// Global variable definitions
instruction_word_t PC = 0;            // Initialize Program Counter to 0
data_word_t SREG = 0;                 // Initialize Status Register to 0

int main() {
    printf("Computer Architecture Simulator Starting...\n");
    
    // Initialize all memory and registers
    init_memory();
    
    // Load and parse assembly program directly into instruction memory
uint16_t program_size = parse_and_load_assembly_file("CSEN.txt");
    printf("Successfully loaded %d instructions into memory\n", program_size);
    
    // Print the instruction memory contents
    printf("\nInstruction Memory Contents:\n");
    printf("-------------------------------------------\n");
    for (uint16_t i = 0; i < program_size; i++) {
        instruction_word_t instr = read_instruction(i);
        printf("Address %03d: 0x%04X  ", i, instr);
        
        // Print binary representation
        for (int j = 15; j >= 0; j--) {
            printf("%d", (instr >> j) & 1);
            if (j % 4 == 0 && j > 0) printf(" ");
        }
        printf("\n");
    }
    
    // Print initial register states
    printf("\nInitial Register States:\n");
    printf("-------------------------------------------\n");
    for (uint8_t i = 0; i < 16; i++) {  // Only printing first 16 registers for brevity
        printf("R%02d: 0x%08X (%d)\n", i, read_register(i), read_register(i));
    }
    
    // Simple execution simulation (non-pipelined)
    printf("\nStarting Execution...\n");
    printf("-------------------------------------------\n");
    
    PC = 0;  // Reset program counter
    while (PC < program_size) {
        // Fetch
        instruction_word_t instr = read_instruction(PC);
        printf("PC: %03d - Executing instruction: 0x%04X\n", PC, instr);
        
        // Simple decode and execute (for testing)
        uint8_t opcode = (instr >> 12) & 0xF;
        uint8_t r1 = (instr >> 6) & 0x3F;
        uint8_t r2 = instr & 0x3F;
        int16_t imm = instr & 0xFF;
        
        switch(opcode) {
            case MOVI:
                write_register(r1, imm);
                printf("  MOVI R%d, %d  -> R%d = %d\n", r1, imm, r1, imm);
                break;
            case ADD: {
                data_word_t val1 = read_register(r1);
                data_word_t val2 = read_register(r2);
                data_word_t result = val1 + val2;
                write_register(r1, result);
                printf("  ADD R%d, R%d  -> R%d = %d + %d = %d\n", 
                      r1, r2, r1, val1, val2, result);
                break;
            }
            case EOR: {
                data_word_t val1 = read_register(r1);
                data_word_t val2 = read_register(r2);
                data_word_t result = val1 ^ val2;
                write_register(r1, result);
                printf("  EOR R%d, R%d  -> R%d = %d ^ %d = %d\n", 
                      r1, r2, r1, val1, val2, result);
                break;
            }
            // Add cases for other instructions as needed...
            default:
                printf("  Unknown opcode: %d\n", opcode);
        }
        
        PC++;  // Move to next instruction
    }
    
    // Print final register states
    printf("\nFinal Register States:\n");
    printf("-------------------------------------------\n");
    for (uint8_t i = 0; i < 16; i++) {
        printf("R%02d: 0x%08X (%d)\n", i, read_register(i), read_register(i));
    }
    
    // Print data memory (showing stored values)
    printf("\nData Memory Contents:\n");
    printf("-------------------------------------------\n");
    for (uint16_t addr = 96; addr < 104; addr++) {  // Showing addresses around 100
        printf("MEM[%03d]: 0x%08X (%d)\n", addr, read_data(addr), read_data(addr));
    }
    
    return 0;
}