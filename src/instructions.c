#include "instructions.h"

// Helper function to update the Carry flag (C)
void update_carry_flag(int8_t result) {
    if (result > 255) {
        SREG |= 0b00000001; // Set the carry flag
    } else {
        SREG &= ~0b00000001; // Clear the carry flag
    }
}

// Helper function to update the Overflow flag (V)
void update_overflow_flag(int8_t destination, int8_t source, int8_t result) {
    if ((destination & 0b10000000) == (source & 0b10000000) && (result & 0b10000000) != (destination & 0b10000000)) {
        SREG |= 0b00000010; // Set the overflow flag
    } else {
        SREG &= ~0b00000010; // Clear the overflow flag
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

void ADD(uint8_t rd, uint8_t rs) {
    // Add the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination + source;

    // Update relevant flags for ADD
    update_carry_flag(result);
    update_overflow_flag(destination, source, result);
    update_negative_flag(result);
    update_sign_flag();
    update_zero_flag(result);

    write_register(rd, (int8_t) result);
}

void SUB(uint8_t rd, uint8_t rs) {
    // Subtract the values in registers rs from rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int8_t result = destination - source;

    // Update relevant flags for SUB
    update_overflow_flag(destination, source, result);
    update_negative_flag(result);
    update_sign_flag();
    update_zero_flag(result);

    write_register(rd, result);
}