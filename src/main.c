#include <stdio.h>
#include "globals.h"
#include "pipeline.h"
#include "memory.h"
#include "parser.h"

// Global variable definitions
instruction_word_t PC = 0; // Initialize Program Counter to 0
data_word_t SREG = 0;      // Initialize Status Register to 0

queue if_id_queue;
queue id_ex_queue;
int sys_call = 1;

int main()
{
    printf("Computer Architecture Simulator Starting...\n");

    // Initialize all memory and registers
    init_memory();

    // Initialize the queues as global variables
    if_id_queue = *(createQueue()); // Instruction Fetch to Decode stage
    id_ex_queue = *(createQueue()); // Decode to Execute stage

    // Load and parse assembly program directly into instruction memory
    uint16_t program_size = parse_and_load_assembly_file("../src/CSEN.txt");

    // Print the instruction memory contents after parsing
    printf("\nInstruction Memory Contents:\n");
    printf("-------------------------------------------\n");
    print_instruction_memory();
    printf("-------------------------------------------\n");
    // Print initial register states
    printf("\nInitial Register States:\n");
    printf("-------------------------------------------\n");
    for (uint8_t i = 0; i < 16; i++) {  // Only printing first 16 registers for brevity
        printf("R%02d: 0x%08X (%d)\n", i, read_register(i), read_register(i));
    }    // Simple execution simulation
    printf("\nStarting Simulation...\n");
    printf("-------------------------------------------\n");

    PC = 0; // Reset program counter
    int sys_call = 1;
    while (stop < 3 && sys_call == 1) // Continue until all instructions are executed
    {
        // Execute one cycle of the pipeline
        pipeline_cycle();
    }

    // Print final simulation results
    printf("\n\n===========================================\n");
    printf("SIMULATION COMPLETE - FINAL RESULTS\n");
    printf("===========================================\n");
    
    // Print final register states
    printf("\nFinal Register States:\n");
    printf("-------------------------------------------\n");
    // Print all general purpose registers
    for (uint8_t i = 0; i < REG_COUNT; i++) {
        printf("R%02d: 0x%02X (%d)\n", i, read_register(i), read_register(i));
    }
    
    // Print special purpose registers
    printf("\nSpecial Purpose Registers:\n");
    printf("-------------------------------------------\n");
    printf("PC: 0x%04X (%d)\n", PC, PC);
    
    // Print SREG bit by bit
    printf("SREG: 0x%02X (", SREG);
    // Show flags - C V N S Z are the flag bits (assuming they're bits 0-4)
    printf("%s", (SREG & 0x01) ? "C" : "-");  // Carry flag
    printf("%s", (SREG & 0x02) ? "V" : "-");  // Overflow flag
    printf("%s", (SREG & 0x04) ? "N" : "-");  // Negative flag
    printf("%s", (SREG & 0x08) ? "S" : "-");  // Sign flag
    printf("%s", (SREG & 0x10) ? "Z" : "-");  // Zero flag
    printf(")\n");
    
    // Print data memory (showing stored values)
    printf("\nData Memory Contents:\n");
    printf("-------------------------------------------\n");
    print_data_memory();
    
    // Print instruction memory contents
    printf("\nInstruction Memory Contents:\n");
    printf("-------------------------------------------\n");
    print_instruction_memory();
    
    printf("\n===========================================\n");
    printf("END OF SIMULATION\n");
    printf("===========================================\n");

    return 0;
}