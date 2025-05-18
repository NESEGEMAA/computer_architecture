/**
 * parser.h
 * Header file for the parser module of Package 3: Double Big Harvard combo large arithmetic shifts
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "memory.h" 

// Define the instruction types
typedef enum
{
    R_TYPE, // Register type
    I_TYPE  // Immediate type
} InstructionType;

// Define opcodes for each instruction

// Structure to represent a parsed instruction
typedef struct
{
    Opcode opcode;
    InstructionType type;
    uint8_t r1;
    uint8_t r2;
    int16_t immediate;
    uint16_t binary; // The binary representation of the instruction
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