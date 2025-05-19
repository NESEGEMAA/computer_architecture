#include "memory.h"
#include <stdio.h>  // For fprintf
#include <stdlib.h> // For exit
#include "pipeline.h"

data_word_t register_file[REG_COUNT];               // Register file (R0-R63)
data_word_t data_memory[DATA_MEMORY_SIZE];          // Data memory
instruction_word_t instr_memory[INSTR_MEMORY_SIZE]; // Instruction memory

// Function to initialize instruction memory
void init_instr_memory()
{
    for (int16_t i = 0; i < INSTR_MEMORY_SIZE; i++)
    {
        instr_memory[i] = UNDEFINED_INT16; // Initialize all instructions to 0
    }
}

// Function to initialize data memory
void init_data_memory()
{
    for (int16_t i = 0; i < DATA_MEMORY_SIZE; i++)
    {
        data_memory[i] = 0; // Initialize all data memory to 0
    }
}

// Function to initialize register file
void init_register_file()
{
    for (uint16_t i = 0; i < REG_COUNT; i++)
    {
        register_file[i] = 0; // Initialize all registers to 0
    }
}

void init_memory()
{
    init_instr_memory();  // Initialize instruction memory
    init_data_memory();   // Initialize data memory
    init_register_file(); // Initialize register file
}

// Function to read an instruction from instruction memory
instruction_word_t read_instruction(uint16_t address)
{
    if (address < INSTR_MEMORY_SIZE)
    {
        return instr_memory[address];
    }
    else
    {
        fprintf(stderr, "Error: Instruction memory read out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}


void write_instruction(uint16_t address, instruction_word_t value)
{
    if (address < INSTR_MEMORY_SIZE)
    {
        instr_memory[address] = value;
    }
    else
    {
        fprintf(stderr, "Error: Instruction memory write out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}

// Function to read data from data memory
data_word_t read_data(uint16_t address)
{
    if (address < DATA_MEMORY_SIZE)
    {
        return data_memory[address];
    }
    else
    {
        fprintf(stderr, "Error: Data memory read out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}

// Function to write data to data memory
void write_data(uint16_t address, data_word_t value)
{
    if (address < DATA_MEMORY_SIZE)
    {
        data_memory[address] = value;
    }
    else
    {
        fprintf(stderr, "Error: Data memory write out of bounds at address %u\n", address);
        exit(EXIT_FAILURE);
    }
}

// Function to read a register value
data_word_t read_register(uint8_t reg_num)
{
    if (reg_num < REG_COUNT && reg_num >= 0)
    {
        return register_file[reg_num];
    }
    else
    {
        fprintf(stderr, "Error: Register read out of bounds at register %u\n", reg_num);
        exit(EXIT_FAILURE);
    }
}

// Function to write a value to a register
void write_register(uint8_t reg_num, data_word_t value)
{
    if (reg_num < REG_COUNT && reg_num >= 0)
    {
        register_file[reg_num] = value;
    }
    else
    {
        fprintf(stderr, "Error: Register write out of bounds at register %u\n", reg_num);
        exit(EXIT_FAILURE);
    }
}

// Function to print data memory contents in formatted form
void print_instruction_memory(void)
{
    printf("\n[Instruction MEMORY]\n");
    int memory_found = 0;

    for (uint16_t i = 0; i < INSTR_MEMORY_SIZE; i++)
    {
        instruction_word_t value = instr_memory[i];

        // Skip if memory location is UNDEFINED_INT16
        if (value == UNDEFINED_INT16)
            continue;

        memory_found = 1;
        // Print address and hex representation
        printf("Address 0x%04X: 0x%04X ", i, (uint16_t)value); // Cast to uint16_t for proper display
        // First 4 bits
        for (int bit = 15; bit >= 12; bit--)
        {
            printf("%d", ((uint16_t)value >> bit) & 0x1);
        }
        printf(" ");

        // Middle 6 bits
        for (int bit = 11; bit >= 6; bit--)
        {
            printf("%d", ((uint16_t)value >> bit) & 0x1);
        }
        printf(" ");

        // Last 6 bits
        for (int bit = 5; bit >= 0; bit--)
        {
            printf("%d", ((uint16_t)value >> bit) & 0x1);
        }
        printf("\n");
    }

    if (!memory_found)
    {
        printf("No memory locations with non-zero values found.\n");
    }
}

void print_data_memory(void)
{
    printf("\n[Data MEMORY]\n");
    int memory_found = 0;

    for (uint16_t i = 0; i < DATA_MEMORY_SIZE; i++)
    {
        data_word_t value = data_memory[i];

        // Skip if memory location is zero
        if (value == 0)
            continue;

        memory_found = 1;
        // Print address and hex representation
        printf("Address 0x%04X: 0x%04X ", i, value);
        // First 4 bits
        for (int bit = 8; bit >= 5; bit--)
        {
            printf("%d", (value >> bit) & 0x1);
        }

        printf(" ");
        for (int bit = 4; bit >= 0; bit--)
        {
            printf("%d", (value >> bit) & 0x1);
        }

        printf("\n");
    }

    if (!memory_found)
    {
        printf("No memory locations with non-zero values found.\n");
    }
}