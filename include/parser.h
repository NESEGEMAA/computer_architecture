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

// Define the instruction types
typedef enum {
    R_TYPE, // Register type
    I_TYPE  // Immediate type
} InstructionType;

// Define opcodes for each instruction
typedef enum {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    MOVI = 3,
    BEQZ = 4,
    ANDI = 5,
    EOR = 6,
    BR = 7,
    SAL = 8,
    SAR = 9,
    LDR = 10,
    STR = 11
} Opcode;

// Structure to represent a parsed instruction
typedef struct {
    Opcode opcode;
    InstructionType type;
    uint8_t r1;
    uint8_t r2;
    int16_t immediate;
    uint16_t binary; // The binary representation of the instruction
} Instruction;

/**
 * Parses the assembly file and returns an array of instructions
 * 
 * @param filename the name of the assembly file
 * @param instr_count pointer to store the number of instructions
 * @return array of instructions
 */
Instruction* parse_assembly_file(const char* filename, int* instr_count);

/**
 * Prints the parsed instruction in human-readable format
 * 
 * @param instr the instruction to print
 */
void print_instruction(const Instruction* instr);

/**
 * Frees the memory allocated for the instructions array
 * 
 * @param instructions the array of instructions to free
 */
void free_instructions(Instruction* instructions);

#endif /* PARSER_H */