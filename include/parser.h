#ifndef PARSER_H
#define PARSER_H

#include "globals.h"
#include <stdint.h>
#include <string.h>
#include "memory.h" 

// Define the instruction types
// typedef enum
// {
//     R_TYPE, // Register type
//     I_TYPE  // Immediate type
// } InstructionType;

// Define opcodes for each instruction

// Structure to represent a parsed instruction
typedef struct
{
    Opcode opcode;
    // InstructionType type;
    uint8_t operand_1;
    uint8_t operand_2;
    // int16_t immediate;
    // uint16_t binary; // The binary representation of the instruction
} InstructionParser;

/**
 * Parses the assembly file and returns an array of instructions
 *
 * @param file_path the full path to the assembly file
 * @param instr_count pointer to store the number of instructions
 * @return array of instructions
 */
InstructionParser *parse_assembly_file(const char *file_path, int *instr_count);

/**
 * Prints the parsed instruction in human-readable format
 *
 * @param instr the instruction to print
 */
void print_instruction(const InstructionParser *instr);

/**
 * Frees the memory allocated for the instructions array
 *
 * @param instructions the array of instructions to free
 */
void free_instructions(InstructionParser *instructions);
uint16_t parse_and_load_assembly_file(const char *file_path);

#endif /* PARSER_H */