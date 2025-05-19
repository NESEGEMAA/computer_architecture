#ifndef INSTRUCTION_MAP_H
#define INSTRUCTION_MAP_H

#include <stdint.h>
#include <string.h>

// Enum for instructions
typedef enum
{
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
    STR = 11,
    INVALID_INSTRUCTION
} Opcode;

// Alias
typedef Opcode Instruction;

// Function to get the enum value from instruction text
Instruction get_instruction_enum(const char* instruction_text);

#endif // INSTRUCTION_MAP_H
