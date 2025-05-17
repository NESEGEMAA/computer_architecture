#include "pipelining.h"
#include "memory.h"
#include "instructions.h"
#include "instruction_map.h"
#include <stdio.h>
#include <stdbool.h>

// Global pipeline stages for the 3-stage pipeline (Instruction Fetch, Decode, Execute)
// Each stage holds instruction data and state, defined by PipelineStage in pipelining.h
PipelineStage fetch_stage = {0};   // Fetch stage: Retrieves instruction from memory
PipelineStage decode_stage = {0};  // Decode stage: Extracts opcode, registers, and immediate
PipelineStage execute_stage = {0}; // Execute stage: Performs the instruction's operation

// Arrays to track register and memory updates for printing (required by project, page 27)
RegisterUpdate reg_updates[REG_COUNT];// reg_updates tracks changes to the 64 registers (R0-R63)
MemoryUpdate mem_updates[DATA_MEMORY_SIZE];// mem_updates tracks changes to the 2048-byte data memory
bool sreg_updated = false;// sreg_updated flags when SREG (Status Register) changes; sreg_new_value stores the new value
data_word_t sreg_new_value;

// Initialize the pipeline and memory
// Called at the start of simulation to reset all pipeline stages and memory
void init_pipeline(void) {
    fetch_stage.valid = false;    // Clear fetch stage: No instruction, invalid state
    decode_stage.valid = false;    // Clear decode stage: No decoded data, invalid state
    execute_stage.valid = false;    // Clear execute stage: No operation to perform, invalid state

    // Initialize register update tracking: No updates pending
    for (int i = 0; i < REG_COUNT; i++) {
        reg_updates[i].updated = false;
    }
    // Initialize memory update tracking: No updates pending
    for (int i = 0; i < DATA_MEMORY_SIZE; i++) {
        mem_updates[i].updated = false;
    }
    sreg_updated = false;// Clear SREG update flag
    PC = 0;// Reset Program Counter (PC) to 0 (start of instruction memory)
    init_memory();// Initialize memory arrays (registers, data memory, instruction memory) to 0

}

// Print register, SREG, and memory updates after each cycle
// Required to show changes in the Execute stage (page 27)
void print_updates(void) {
    // Check all 64 registers for updates
    for (int i = 0; i < REG_COUNT; i++) {
        if (reg_updates[i].updated) {
            printf("Register R%d updated to %d in EX\n", i, reg_updates[i].new_value);// Print register update (e.g., "Register R5 updated to 10 in EX")
            reg_updates[i].updated = false;// Clear update flag to avoid reprinting
        }
    }
    // Check if SREG was updated
    if (sreg_updated) {
        // Print SREG bits: Carry (C), Overflow (V), Negative (N), Sign (S), Zero (Z)
        printf("SREG updated to C=%d, V=%d, N=%d, S=%d, Z=%d in EX\n",
               sreg_new_value & 0x1,           // Bit 0: Carry
               (sreg_new_value >> 1) & 0x1,    // Bit 1: Overflow
               (sreg_new_value >> 2) & 0x1,    // Bit 2: Negative
               (sreg_new_value >> 3) & 0x1,    // Bit 3: Sign
               (sreg_new_value >> 4) & 0x1);   // Bit 4: Zero
        sreg_updated = false;// Clear SREG update flag
    }
    // Check all data memory locations for updates
    for (int i = 0; i < DATA_MEMORY_SIZE; i++) {
        if (mem_updates[i].updated) {            
            printf("Data Memory[%d] updated to %d in EX\n", i, mem_updates[i].new_value);// Print memory update (e.g., "Data Memory[100] updated to 10 in EX")
            mem_updates[i].updated = false;// Clear update flag to avoid reprinting
        }
    }
}

// Execute the instruction in the Execute stage
// Uses the decoded opcode to call the appropriate instruction function
void execute_instruction(PipelineStage *stage) {
    // Print the opcode being executed
    printf("EX: Executing opcode=%u ", stage->opcode);
    // Switch on the opcode (using numeric values, not Instruction enum)
    switch (stage->opcode) {
        case 0: // ADD: Add two registers (Rd += Rs)
            printf("(ADD R%d R%d)\n", stage->r1, stage->r2);
            _ADD(stage->r1, stage->r2);
            break;
        case 1: // SUB: Subtract two registers (Rd -= Rs)
            printf("(SUB R%d R%d)\n", stage->r1, stage->r2);
            _SUB(stage->r1, stage->r2);
            break;
        case 2: // MUL: Multiply two registers (Rd *= Rs)
            printf("(MUL R%d R%d)\n", stage->r1, stage->r2);
            _MUL(stage->r1, stage->r2);
            break;
        case 3: // MOVI: Move immediate to register (Rd = imm)
            printf("(MOVI R%d %d)\n", stage->r1, stage->imm);
            _MOVI(stage->r1, stage->imm);
            break;
        case 4: // BEQZ: Branch if register is zero (if Rs == 0, PC = PC + 1 + imm)
            printf("(BEQZ R%d %d)\n", stage->r1, stage->imm);
            // Correct PC update: Use stored PC from fetch
            if (read_register(stage->r1) == 0) {
                PC = stage->pc + 1 + stage->imm;
            }
            break;
        case 5: // ANDI: AND register with immediate (Rd &= imm)
            printf("(ANDI R%d %d)\n", stage->r1, stage->imm);
            _ANDI(stage->r1, stage->imm);
            break;
        case 6: // EOR: Exclusive OR two registers (Rd ^= Rs)
            printf("(EOR R%d R%d)\n", stage->r1, stage->r2);
            _EOR(stage->r1, stage->r2);
            break;
        case 7: // BR: Branch to address in two registers (PC = (R1 << 8) | R2)
            printf("(BR R%d R%d)\n", stage->r1, stage->r2);
            _BR(stage->r1, stage->r2); // Use _BR from instructions.c
            break;
        case 8: // SAL: Shift arithmetic left (Rd <<= imm)
            printf("(SAL R%d %d)\n", stage->r1, stage->imm);
            _SAL(stage->r1, stage->imm); // Implemented
            break;
        case 9: // SAR: Shift arithmetic right (Rd >>= imm)
            printf("(SAR R%d %d)\n", stage->r1, stage->imm);
            _SAR(stage->r1, stage->imm); // Implemented
            break;
        case 10: // LDR: Load from data memory (Rd = MEM[imm])
            printf("(LDR R%d %d)\n", stage->r1, stage->imm);
            _LDR(stage->r1, stage->imm); // Implemented
            break;
        case 11: // STR: Store to data memory (MEM[imm] = Rd)
            printf("(STR R%d %d)\n", stage->r1, stage->imm);
            _STR(stage->r1, stage->imm); // Implemented
            break;
        default:
            // Handle invalid opcodes (0–11 are valid, per page 17)
            printf("(Invalid)\n");
            break;
    }
}

// Run the 3-stage pipeline simulation
// Continues until all stages are empty and no more instructions remain
void run_pipeline(void) {
    int cycle = 0;// Cycle counter for tracking simulation steps

    // Run while any stage is active or instructions remain in memory
    while (fetch_stage.valid || decode_stage.valid || execute_stage.valid || PC < INSTR_MEMORY_SIZE) {
        cycle++;
        // Print cycle number for debugging and output
        printf("\nCycle %d:\n", cycle);

        // Shift pipeline stages: EX <- ID <- IF
        // Move decode stage to execute, fetch stage to decode
        execute_stage = decode_stage;
        decode_stage = fetch_stage;
        fetch_stage.valid = false;// Clear fetch stage for new instruction

        // Instruction Fetch (IF) stage
        // Fetch next instruction if PC is within instruction memory (1024 words)
        if (PC < INSTR_MEMORY_SIZE) {
            fetch_stage.instr = read_instruction(PC);//Read instruction from memory at PC
            fetch_stage.pc = PC;// Store current PC for use in BEQZ
            fetch_stage.valid = true;// Mark fetch stage as active
            // Print fetch details (e.g., "IF: Fetched instruction at PC=0: 0x300A")
            printf("IF: Fetched instruction at PC=%d: 0x%04x\n", PC, fetch_stage.instr);
            // Increment PC to next instruction
            PC++;
        }

        // Instruction Decode (ID) stage
        // Decode instruction if decode stage is active
        if (decode_stage.valid) {
            // Extract opcode (bits 15–12, 4 bits)
            decode_stage.opcode = (decode_stage.instr >> 12) & 0xF;
            // Extract r1 (Rd, bits 11–6, 6 bits)
            decode_stage.r1 = (decode_stage.instr >> 6) & 0x3F;
            // Extract r2 or immediate (bits 5–0, 6 bits)
            decode_stage.r2 = decode_stage.instr & 0x3F;
            // Sign-extend immediate for instructions like BEQZ, MOVI
            decode_stage.imm = (int8_t)(decode_stage.r2 & 0x3F);
            // Print decode details (e.g., "ID: Decoded opcode=3, r1=1, r2/imm=10")
            printf("ID: Decoded opcode=%u, r1=%u, r2/imm=%d\n", decode_stage.opcode, decode_stage.r1, decode_stage.imm);
        }

        // Execute (EX) stage
        // Execute instruction if execute stage is active
        if (execute_stage.valid) {
            execute_instruction(&execute_stage);
        }

        // Print any updates (registers, SREG, memory) from the execute stage
        print_updates();
    }
    // Print final state after simulation completes
    print_final_state();
}

// Print the state of each pipeline stage for a given cycle
// Used for debugging and visualization (page 27)
void print_pipeline_state(int cycle) {
    // Print cycle number
    printf("Cycle %d Pipeline State:\n", cycle);
    // Fetch stage status
    if (fetch_stage.valid) {
        // Show PC and instruction if active
        printf("IF: PC=%d, Instruction=0x%04x\n", fetch_stage.pc, fetch_stage.instr);
    } else {
        // Indicate idle if no instruction
        printf("IF: Idle\n");
    }
    // Decode stage status
    if (decode_stage.valid) {
        // Show decoded opcode, r1, and r2/imm if active
        printf("ID: Opcode=%u, R1=%u, R2/IMM=%d\n", decode_stage.opcode, decode_stage.r1, decode_stage.imm);
    } else {
        // Indicate idle if no decoded data
        printf("ID: Idle\n");
    }
    // Execute stage status
    if (execute_stage.valid) {
        // Show opcode being executed
        printf("EX: Opcode=%u\n", execute_stage.opcode);
    } else {
        // Indicate idle if no execution
        printf("EX: Idle\n");
    }
}

// Print the final state of registers, SREG, PC, and memory
// Required at the end of simulation (page 27)
void print_final_state(void) {
    printf("\nFinal State:\n");
    // Print all 64 registers
    printf("Registers:\n");
    for (int i = 0; i < REG_COUNT; i++) {
        printf("R%d: %d\n", i, read_register(i));
    }
    // Print SREG bits: Carry, Overflow, Negative, Sign, Zero
    printf("SREG: C=%d, V=%d, N=%d, S=%d, Z=%d\n",
           SREG & 0x1,           // Bit 0
           (SREG >> 1) & 0x1,    // Bit 1
           (SREG >> 2) & 0x1,    // Bit 2
           (SREG >> 3) & 0x1,    // Bit 3
           (SREG >> 4) & 0x1);   // Bit 4
    // Print final PC value
    printf("PC: %d\n", PC);
    // Print non-zero instruction memory entries
    printf("Instruction Memory:\n");
    for (int i = 0; i < INSTR_MEMORY_SIZE; i++) {
        if (instr_memory[i] != 0) {
            printf("MEM[%d]: 0x%04x\n", i, instr_memory[i]);
        }
    }
    // Print non-zero data memory entries
    printf("Data Memory:\n");
    for (int i = 0; i < DATA_MEMORY_SIZE; i++) {
        if (data_memory[i] != 0) {
            printf("MEM[%d]: %d\n", i, data_memory[i]);
        }
    }
}