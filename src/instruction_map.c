#include "instruction_map.h"

// Function to get the enum value from instruction text
Instruction get_instruction_enum(const char* instruction_text) {
    if (strcmp(instruction_text, "ADD") == 0) return ADD;
    if (strcmp(instruction_text, "SUB") == 0) return SUB;
    if (strcmp(instruction_text, "MUL") == 0) return MUL;
    if (strcmp(instruction_text, "MOVI") == 0) return MOVI;
    if (strcmp(instruction_text, "BEQZ") == 0) return BEQZ;
    if (strcmp(instruction_text, "ANDI") == 0) return ANDI;
    if (strcmp(instruction_text, "EOR") == 0) return EOR;
    if (strcmp(instruction_text, "BR") == 0) return BR;
    if (strcmp(instruction_text, "SAL") == 0) return SAL;
    if (strcmp(instruction_text, "SAR") == 0) return SAR;
    if (strcmp(instruction_text, "LDR") == 0) return LDR;
    if (strcmp(instruction_text, "STR") == 0) return STR;

    return INVALID_INSTRUCTION; // Return invalid for unrecognized instructions
}
