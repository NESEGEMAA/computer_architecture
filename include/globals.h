#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // For fixed-width integer types

// Type definitions for assembler
typedef uint8_t  data_word_t;     // 8-bit data word
typedef uint16_t instruction_t;    // 16-bit instruction word

// Global variable declarations
extern instruction_t PC;          // Program Counter
extern data_word_t SREG;         // Status Register

#endif // GLOBALS_H