// types.h - Updated version
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "instruction_map.h"

typedef int8_t data_word_t;         // 8-bit data word
typedef uint16_t instruction_word_t; // 16-bit instruction word
#define UNDEFINED_INT16 32768

// Forward declarations for recursive pointers
typedef struct IF_ID IF_ID;
typedef struct ID_EX ID_EX;
typedef struct EXEC EXEC; // Changed from EX to EXEC as the type name

struct IF_ID {
    instruction_word_t instr;
    uint16_t pc;
    IF_ID *next;
};

struct ID_EX {
    instruction_word_t instruction;
    instruction_word_t pc;
    Opcode opcode;
    uint8_t r1, r2;
    data_word_t r1_value, r2_value;
    int8_t immediate;
    ID_EX *next;
    int data_hazard;
    int r1_forward;
    int r2_forward;
};

struct EXEC {
    data_word_t result; 
};

#endif // TYPES_H