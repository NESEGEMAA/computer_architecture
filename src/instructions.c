#include "instructions.h"
#include "pipelining.h"

// Helper function to update the Carry flag (C)
void update_carry_flag(int8_t result)
{
    if (result > 255)
    {
        SREG |= 0b00000001; // Set the carry flag
    }
    else
    {
        SREG &= ~0b00000001; // Clear the carry flag
    }
}

// Helper function to update the Overflow flag (V)
void update_overflow_flag(Instruction instruction, int8_t destination, int8_t source, int8_t result)
{
    if (instruction == ADD)
    {
        if (((destination > 0) && (source > 0) && (result < 0)) || ((destination < 0) && (source < 0) && (result > 0)))
        {
            SREG |= 0b00000010; // Set the overflow flag
        }
        else
        {
            SREG &= ~0b00000010; // Clear the overflow flag
        }
    }
    else if (instruction == SUB)
    {
        if (((destination < 0) && (source > 0) && (result > 0)) || ((destination > 0) && (source < 0) && (result < 0)))
        {
            SREG |= 0b00000010; // Set the overflow flag
        }
        else
        {
            SREG &= ~0b00000010; // Clear the overflow flag
        }
    }
}

// Helper function to update the Negative flag (N)
void update_negative_flag(int8_t result)
{
    if (result & 0b10000000)
    {
        SREG |= 0b00000100; // Set the negative flag
    }
    else
    {
        SREG &= ~0b00000100; // Clear the negative flag
    }
}

// Helper function to update the Sign flag (S)
void update_sign_flag()
{
    if (((SREG & 0b00000100) >> 2) ^ ((SREG & 0b00000010) >> 1))
    {
        SREG |= 0b00001000; // Set the sign flag
    }
    else
    {
        SREG &= ~0b00001000; // Clear the sign flag
    }
}

// Helper function to update the Zero flag (Z)
void update_zero_flag(int8_t result)
{
    if (result == 0)
    {
        SREG |= 0b00010000; // Set the zero flag
    }
    else
    {
        SREG &= ~0b00010000; // Clear the zero flag
    }
}

// Helper function to update flags based on the instruction
void update_flags(Instruction instruction, int8_t destination, int8_t source, int16_t result)
{
    switch (instruction)
    {
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

void _ADD(uint8_t rd, uint8_t rs)
{
    // Add the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination + source;

    // Update relevant flags for ADD
    update_flags(ADD, destination, source, result);

    write_register(rd, (int8_t)result);
}

void _SUB(uint8_t rd, uint8_t rs)
{
    // Subtract the values in registers rs from rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination - source;

    // Update relevant flags for SUB
    update_flags(SUB, destination, source, result);

    write_register(rd, (int8_t)result);
}

void _MUL(uint8_t rd, uint8_t rs)
{
    // Multiply the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int8_t source = read_register(rs);
    int16_t result = destination * source;

    // Update relevant flags for MUL
    update_flags(MUL, destination, source, result);

    write_register(rd, (int8_t)result);
}

void _MOVI(uint8_t rd, int8_t immediate)
{
    // Move immediate value to register rd
    write_register(rd, immediate);
}

void _BEQZ(uint8_t rs, int8_t immediate)
{
    // Branch if the value in register rs is zero
    int8_t value = read_register(rs);
    if (value == 0)
    {
        PC += immediate;
    }
}

void _ANDI(uint8_t rd, int8_t immediate)
{
    // AND the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int16_t result = destination & immediate;

    // Update relevant flags for ANDI
    update_flags(ANDI, destination, immediate, result);

    write_register(rd, (int8_t)result);
}

void _EOR(uint8_t rd, uint8_t rs)
{
    // EOR the values in registers rs and rd, store result in rd
    int8_t destination = read_register(rd);
    int16_t result = destination ^ rs;

    // Update relevant flags for EOR
    update_flags(EOR, destination, rs, result);

    write_register(rd, (int8_t)result);
}

void _BR(uint8_t rd, uint8_t rs)
{
    // Branch Register - set the PC to the concatenated value of registers rd and rs
    int8_t high_byte = read_register(rd);
    int8_t low_byte = read_register(rs);

    // Concatenate the two registers to form a 16-bit address
    uint16_t new_pc = ((uint16_t)(uint8_t)high_byte << 8) | (uint8_t)low_byte;

    PC = new_pc;
}

void _SAL(uint8_t rd, int8_t immediate)
{
    // Shift Arithmetic Left - shift the value in rd left by immediate bits
    int8_t destination = read_register(rd);
    int16_t result = 0;

    // Perform shift only if immediate is positive
    if (immediate >= 0)
    {
        result = (int16_t)destination << immediate;
    }
    else
    {
        // Handle negative shift (which should be a right shift)
        result = (int16_t)destination >> (-immediate);
    }

    // Update relevant flags for SAL
    update_flags(SAL, destination, immediate, result);

    write_register(rd, (int8_t)result);
}

void _SAR(uint8_t rd, int8_t immediate)
{
    // Shift Arithmetic Right - shift the value in rd right by immediate bits
    // This preserves the sign bit
    int8_t destination = read_register(rd);
    int16_t result = destination >> immediate;

    // Update relevant flags for SAR
    update_flags(SAR, destination, immediate, result);

    write_register(rd, (int8_t)result);
}

void _LDR(uint8_t rd, uint16_t address)
{
    // Load to Register - load value from memory at address into register rd
    int8_t value = read_memory(address);
    write_register(rd, value);
}

void _STR(uint8_t rd, uint16_t address)
{
    // Store from Register - store value from register rd into memory at address
    int8_t value = read_register(rd);
    write_memory(address, value);
}