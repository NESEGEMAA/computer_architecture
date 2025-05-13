#ifndef INSTRUCTION_MAP_H
#define INSTRUCTION_MAP_H

#include <stdint.h>
#include <string.h>

// Enum for instructions
typedef enum {
    ADD,
    SUB,
    MUL,
    MOVI,
    BEQZ,
    ANDI,
    EOR,
    BR,
    SAL,
    SAR,
    LDR,
    STR,
    INVALID_INSTRUCTION // For error handling
} Instruction;

// Function to get the enum value from instruction text
Instruction get_instruction_enum(const char* instruction_text);

#endif // INSTRUCTION_MAP_H
