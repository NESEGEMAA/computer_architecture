// #ifndef PIPELINING_H
// #define PIPELINING_H

// #include "globals.h"
// #include "instruction_map.h"
// #include <stdbool.h> // Add this line for bool type
// #include "memory.h"

// // Structure to represent a pipeline stage
// typedef struct {
//     instruction_word_t instr; // 16-bit instruction
//     instruction_word_t pc;   // PC at fetch time
//     uint8_t opcode;          // Decoded opcode (0–11)
//     uint8_t r1, r2;          // Register indices
//     int8_t imm;              // Immediate value (signed)
//     bool valid;              // Is this stage active?
// } PipelineStage;

// // Track register and memory updates for printing
// typedef struct {
//     uint8_t reg_num;
//     data_word_t new_value;
//     bool updated;
// } RegisterUpdate;

// typedef struct {
//     uint16_t addr;
//     data_word_t new_value;
//     bool updated;
// } MemoryUpdate;

// // Pipeline stages
// extern PipelineStage fetch_stage;
// extern PipelineStage decode_stage;
// extern PipelineStage execute_stage;
// extern RegisterUpdate reg_updates[REG_COUNT];
// extern MemoryUpdate mem_updates[DATA_MEMORY_SIZE];
// extern bool sreg_updated;
// extern data_word_t sreg_new_value;
// extern data_word_t SREG;    // Status register
// extern uint16_t PC;     // Program counter

// // Pipeline control functions
// void init_pipeline(void);            // Initialize pipeline stages
// void run_pipeline(void);             // Run the pipeline simulation
// void print_pipeline_state(int cycle); // Print pipeline state for a cycle
// void print_final_state(void);        // Print final register and memory state

// #endif // PIPELINING_H