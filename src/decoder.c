#include <stdio.h>
#include "decoder.h"

// Function to check if instruction is R-Format
int is_r_format(uint8_t opcode) {
    return (opcode == ADD ||
        opcode == SUB ||
        opcode == MUL ||
        opcode == EOR ||
        opcode == BR);
}

// Function to check if instruction needs sign extension for immediate
int needs_sign_extension(uint8_t opcode) {
    // SAL and SAR use positive immediates; others use signed immediates
    return (opcode == MOVI ||
        opcode == BEQZ ||
        opcode == ANDI );
}

// Function to decode an instruction
DecodedInstruction decode_stage(uint16_t instruction, uint16_t pc, int cycle) {
    DecodedInstruction decoded;
    decoded.pc = pc;
    decoded.instruction = instruction;

    if (instruction == 0) {
        printf("Cycle %d: Decode stage is empty\n", cycle);
        decoded.opcode = 0;
        decoded.r1 = 0;
        decoded.r2 = 0;
        decoded.immediate = 0;
        decoded.is_r_format = 0;
        return decoded;
    }

    // Extract opcode (bits 15:12)
    decoded.opcode = (instruction >> 12) & 0xF;

    // Determine instruction format
    decoded.is_r_format = is_r_format(decoded.opcode);

    if (decoded.is_r_format) {
        // R-Format: OPCODE (4 bits), R1 (6 bits), R2 (6 bits)
        decoded.r1 = (instruction >> 6) & 0x3F; // bits 11-6
        decoded.r2 = instruction & 0x3F;        // bits 5-0
        decoded.immediate = 0;                   // not used
    }
    else {
        // I-Format: OPCODE (4 bits), R1 (6 bits), IMMEDIATE (6 bits)
        decoded.r1 = (instruction >> 6) & 0x3F; // bits 11-6
        decoded.r2 = 0;                         // not used
        // Extract 6-bit immediate
        uint8_t imm = instruction & 0x3F;       // bits 5-0
        // Sign-extend if needed (for MOVI, BEQZ, ANDI, LDR, STR)
        if (needs_sign_extension(decoded.opcode)) {
            decoded.immediate = (imm & 0x20) ? (imm | 0xC0) : imm; // Sign-extend to 8 bits
        }
        else {
            decoded.immediate = imm; // Positive immediate for SAL, SAR
        }
    }

    // Print decode stage information (register numbers only, no values)
    printf("Cycle %d: Decode Stage\n", cycle);
    printf("  Instruction: 0x%04X\n", instruction);
    printf("  Opcode: %u\n", decoded.opcode);
    printf("  Format: %s\n", decoded.is_r_format ? "R-Format" : "I-Format");
    printf("  R1: R%u\n", decoded.r1);
    if (decoded.is_r_format) {
        printf("  R2: R%u\n", decoded.r2);
    }
    else {
        printf("  Immediate: %d\n", decoded.immediate);
    }
    printf("  PC: %u\n", decoded.pc);

    return decoded;
}

// Main function for testing decode stage with CSEN.txt
// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <assembly_file>\n", argv[0]);
//         return 1;
//     }

//     int instr_count = 0;
//     // Parse the CSEN.txt assembly file
//     Instruction* instructions = parse_assembly_file(argv[1], &instr_count);

//     if (instr_count == 0) {
//         printf("No instructions parsed from %s.\n", argv[1]);
//         free_instructions(instructions);
//         return 1;
//     }

//     // Test decode stage for each parsed instruction
//     for (int i = 0; i < instr_count; i++) {
//         // Print the parsed instruction for reference
//         printf("\nParsed Instruction %d:\n", i + 1);
//         print_instruction(&instructions[i]);

//         // Decode the instruction (use index as PC, i+1 as cycle)
//         DecodedInstruction decoded = decode_stage(instructions[i].binary, i, i + 1);
//     }

//     // Free the parsed instructions
//     free_instructions(instructions);
//     return 0;
// }

//Expected Output:
/*Cycle 1:
Parsed Instruction :
Opcode: MOVI(3), Type : I, R1 : R1, Immediate : 10, Binary : 0xC04A
Cycle 1 : Decode Stage
Instruction : 0xC04A
Opcode : 3
Format : I - Format
R1 : R1
Immediate : 10
PC : 0

Cycle 2 :
    Parsed Instruction :
Opcode: MOVI(3), Type : I, R1 : R2, Immediate : 12, Binary : 0xC08C
Cycle 2 : Decode Stage
Instruction : 0xC08C
Opcode : 3
Format : I - Format
R1 : R2
Immediate : 12
PC : 1

Cycle 3 :
    Parsed Instruction :
Opcode: EOR(6), Type : R, R1 : R3, R2 : R1, Binary : 0x60C1
Cycle 3 : Decode Stage
Instruction : 0x60C1
Opcode : 6
Format : R - Format
R1 : R3
R2 : R1
PC : 2

Cycle 4 :
    Parsed Instruction :
Opcode: ADD(0), Type : R, R1 : R4, R2 : R3, Binary : 0x0103
Cycle 4 : Decode Stage
Instruction : 0x0103
Opcode : 0
Format : R - Format
R1 : R4
R2 : R3
PC : 3

Cycle 5 :
    Parsed Instruction :
Opcode: SUB(1), Type : R, R1 : R5, R2 : R1, Binary : 0x4141
Cycle 5 : Decode Stage
Instruction : 0x4141
Opcode : 1
Format : R - Format
R1 : R5
R2 : R1
PC : 4

Cycle 6 :
    Parsed Instruction :
Opcode: STR(11), Type : I, R1 : R3, Immediate : 31, Binary : 0xB0DF
Cycle 6 : Decode Stage
Instruction : 0xB0DF
Opcode : 11
Format : I - Format
R1 : R3
Immediate : 31
PC : 5

Cycle 7 :
    Parsed Instruction :
Opcode: LDR(10), Type : I, R1 : R6, Immediate : 31, Binary : 0xA17F
Cycle 7 : Decode Stage
Instruction : 0xA17F
Opcode : 10
Format : I - Format
R1 : R6
Immediate : 31
PC : 6

Cycle 8 :
    Parsed Instruction :
Opcode: BEQZ(4), Type : I, R1 : R6, Immediate : 2, Binary : 0xD182
Cycle 8 : Decode Stage
Instruction : 0xD182
Opcode : 4
Format : I - Format
R1 : R6
Immediate : 2
PC : 7*/