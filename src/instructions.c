#include "instructions.h"
#include "pipelining.h"

// Helper function to update the Carry flag (C)
void update_carry_flag(int8_t result) {
    if (result > 255) {
        SREG |= 0b00000001; // Set the carry flag
    } else {
        SREG &= ~0b00000001; // Clear the carry flag
    }
}

// Helper function to update the Overflow flag (V)
void update_overflow_flag(Instruction instruction, int8_t destination, int8_t source, int8_t result) {
    if (instruction == ADD) {
        if (((destination > 0) && (source > 0) && (result < 0)) || ((destination < 0) && (source < 0) && (result > 0))) {
            SREG |= 0b00000010; // Set the overflow flag
        } else {
            SREG &= ~0b00000010; // Clear the overflow flag
        }
    } else if (instruction == SUB) {
        if (((destination < 0) && (source > 0) && (result > 0)) || ((destination > 0) && (source < 0) && (result < 0))) {
            SREG |= 0b00000010; // Set the overflow flag
        } else {
            SREG &= ~0b00000010; // Clear the overflow flag
        }
    }
}

// Helper function to update the Negative flag (N)
void update_negative_flag(int8_t result) {
    if (result & 0b10000000) {
        SREG |= 0b00000100; // Set the negative flag
    } else {
        SREG &= ~0b00000100; // Clear the negative flag
    }
}

// Helper function to update the Sign flag (S)
void update_sign_flag() {
    if (((SREG & 0b00000100) >> 2) ^ ((SREG & 0b00000010) >> 1)) {
        SREG |= 0b00001000; // Set the sign flag
    } else {
        SREG &= ~0b00001000; // Clear the sign flag
    }
}

// Helper function to update the Zero flag (Z)
void update_zero_flag(int8_t result) {
    if (result == 0) {
        SREG |= 0b00010000; // Set the zero flag
    } else {
        SREG &= ~0b00010000; // Clear the zero flag
    }
}

// Helper function to update flags based on the instruction
void update_flags(Instruction instruction, int8_t destination, int8_t source, int16_t result) {
    switch (instruction) {
        case ADD:
            update_carry_flag(result);
            update_overflow_flag(instruction, destination, source, result);
            update_negative_flag(result);
            update_sign_flag();
            update_zero_flag(result);
            break;
        case SUB:
            update_overflow_flag(instruction, destination, source, result);
            update_negative_flag(result);
            update_sign_flag();
            update_zero_flag(result);
            break;
        case MUL:
        case ANDI:
        case EOR:
        case SAL:
        case SAR:
            update_negative_flag(result);
            update_zero_flag(result);
            break;
        default:
            // No flags to update for other instructions
            break;
    }
}

void _ADD(uint8_t rd, uint8_t rs) {
    // Add the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination + source;

    // Update relevant flags for ADD
    update_flags(ADD, destination, source, result);

    write_register(rd, (int8_t) result);
}

void _SUB(uint8_t rd, uint8_t rs) {
    // Subtract the values in registers rs from rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination - source;

    // Update relevant flags for SUB
    update_flags(SUB, destination, source, result);

    write_register(rd, (int8_t) result);
}

void _MUL(uint8_t rd, uint8_t rs) {
    // Multiply the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination * source;

    // Update relevant flags for MUL
    update_flags(MUL, destination, source, result);

    write_register(rd, (int8_t) result);
}

void _MOVI(uint8_t rd, int8_t immediate) {
    // Move immediate value to register rd
    write_register(rd, immediate);
}

void _BEQZ(uint8_t rs, int8_t immediate) {
    // Branch if the value in register rs is zero
    int8_t value = read_register(rs);
    if (value == 0) {
        PC += immediate; 
    }
}

void _ANDI(uint8_t rd, int8_t immediate) {
    // AND the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int16_t result = destination & immediate;

    // Update relevant flags for ANDI
    update_flags(ANDI, destination, immediate, result);

    write_register(rd, (int8_t) result);
}

void _EOR(uint8_t rd, uint8_t rs) {
    int8_t destination = read_register(rd); // Read Rd value
    int8_t source = read_register(rs);      // Read Rs value
    int16_t result = destination ^ source;  // Perform XOR
    update_flags(EOR, destination, source, result); // Update N, Z flags (page 26)
    write_register(rd, (int8_t)result);     // Write result to Rd
}

// Missing instruction implementations
// void _BR(uint8_t r1, uint8_t r2) {
//     PC = ((uint16_t)read_register(r1) << 8) | read_register(r2);
// }

// void _SAL(uint8_t rd, int8_t imm) {
//     int8_t value = read_register(rd);
//     int16_t result = value << (imm & 0x3F);
//     update_flags(SAL, value, 0, result);
//     write_register(rd, (int8_t)result);
// }

// void _SAR(uint8_t rd, int8_t imm) {
//     int8_t value = read_register(rd);
//     int16_t result = value >> (imm & 0x3F);
//     update_flags(SAR, value, 0, result);
//     write_register(rd, (int8_t)result);
// }

// void _LDR(uint8_t rd, int8_t address) {
//     data_word_t value = read_data(address & 0x3F);
//     write_register(rd, value);
// }

// void _STR(uint8_t rd, int8_t address) {
//     data_word_t value = read_register(rd);
//     write_data(address & 0x3F, value);
// }