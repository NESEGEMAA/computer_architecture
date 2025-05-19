#include <stdio.h>
#include "decoder.h"
#include "pipeline.h"

int is_r_format;

// Function to check if instruction is R-Format
int isit_r_format(uint8_t opcode)
{
    return (opcode == ADD ||
            opcode == SUB ||
            opcode == MUL ||
            opcode == EOR ||
            opcode == BR);
}

// Function to check if instruction needs sign extension for immediate
int needs_sign_extension(uint8_t opcode)
{
    // SAL and SAR use positive immediates; others use signed immediates
    return (opcode == MOVI ||
            opcode == BEQZ ||
            opcode == ANDI);
}

// Function to decode an instruction
void decode_stage()
{
    // Make sure queue is not empty before peeking
    if (isEmpty(&if_id_queue))
    {
        printf("Decode Stage: Stopped\n");
        return;
    }

    IF_ID if_id = *(peek_if_id(&if_id_queue)); // Instruction Fetch to Decode stage
    ID_EX id_ex = {0};   // Decode to Execute stage

    uint16_t instruction = if_id.instr; // Get instruction from IF/ID stage

    id_ex.pc = if_id.pc;
    id_ex.instruction = instruction;

    if (if_id.instr != UNDEFINED_INT16)
    {
        // Extract opcode (bits 15:12)
        id_ex.opcode = (instruction >> 12) & 0xF;

        // Determine instruction format
        is_r_format = isit_r_format(id_ex.opcode);

        if (is_r_format)
        {
            // R-Format: OPCODE (4 bits), R1 (6 bits), R2 (6 bits)
            id_ex.r1 = (instruction >> 6) & 0x3F; // bits 11-6
            id_ex.r2 = instruction & 0x3F;        // bits 5-0
            id_ex.immediate = 0;                  // not used

            id_ex.r1_value = read_register(id_ex.r1); // Read R1 value
            id_ex.r2_value = read_register(id_ex.r2); // Read R2 value
        }
        else
        {
            // I-Format: OPCODE (4 bits), R1 (6 bits), IMMEDIATE (6 bits)
            id_ex.r1 = (instruction >> 6) & 0x3F; // bits 11-6
            id_ex.r2 = 0;                         // not used
            // Extract 6-bit immediate
            uint8_t imm = instruction & 0x3F;         // bits 5-0
            id_ex.r1_value = read_register(id_ex.r1); // Read R1 value

            // Sign-extend if needed (for MOVI, BEQZ, ANDI, LDR, STR)
            if (needs_sign_extension(id_ex.opcode))
            {
                id_ex.immediate = (imm & 0x20) ? (imm | 0xC0) : imm; // Sign-extend to 8 bits
            }
            else
            {
                id_ex.immediate = imm; // Positive immediate for SAL, SAR
            }        
        } 
        // Print decode stage information with input and output values
        printf("Decode Stage:\n");
        printf("  Input: Instruction = 0x%04X from PC = %d\n", instruction, id_ex.pc - 1);
        printf("  Opcode: %u (%s)\n", id_ex.opcode, get_opcode_mnemonic(id_ex.opcode));
        printf("  Format: %s\n", is_r_format ? "R-Format" : "I-Format");
        
        // Print detailed input/output information
        printf("  Output: ");
        if (is_r_format)
        {
            printf("R1: R%u = %d, R2: R%u = %d, PC: %u\n", 
                   id_ex.r1, id_ex.r1_value, 
                   id_ex.r2, id_ex.r2_value,
                   id_ex.pc);
        }
        else
        {
            printf("R1: R%u = %d, Immediate: %d, PC: %u\n", 
                   id_ex.r1, id_ex.r1_value, 
                   id_ex.immediate,
                   id_ex.pc);
        }

        // Print the instruction in human-readable format
        print_decoded_instruction(id_ex.opcode, id_ex.r1, id_ex.r2, id_ex.immediate, is_r_format);

        // Dequeue from IF to ID stage (do this after processing the instruction)
        dequeue_if_id(&if_id_queue);
        if(!isEmpty(&id_ex_queue)){
        ID_EX executing = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
        if (executing.opcode!=BEQZ && executing.opcode!=BR){
            // Check for data hazards
            if ( id_ex.opcode==LDR && executing.opcode==STR&&id_ex.immediate==executing.immediate){
                  id_ex.data_hazard = 1;
                  id_ex.data_stall=0;
                
            }
            else if (id_ex.r1==executing.r1 && (id_ex.opcode!=LDR && id_ex.opcode!=MOVI)){
                 id_ex.data_hazard = 1;
                 id_ex.r1_forward=1;
                 id_ex.data_stall=0;
            }
            else if (id_ex.r2==executing.r1){
                id_ex.data_hazard = 1;
                id_ex.r2_forward=1;
                id_ex.data_stall=0;
            }
        }
       }
       printf("Data hazard:%d:, Data stall: %d...\n", id_ex.data_hazard, id_ex.data_stall);
        // Enqueue to Decode to Execute stage
        enqueue_id_ex(&id_ex_queue, &id_ex);
        // if (!isEmpty(&id_ex_queue)){
        //   printf("To be executed ");
        //   print_queue(&id_ex_queue); // Print the queue after processing
        // }
       
        return;
    }
}

// Function to get opcode mnemonic string
const char *get_opcode_mnemonic(uint8_t opcode)
{
    switch (opcode)
    {
    case ADD:
        return "ADD";
    case SUB:
        return "SUB";
    case MUL:
        return "MUL";
    case MOVI:
        return "MOVI";
    case BEQZ:
        return "BEQZ";
    case ANDI:
        return "ANDI";
    case EOR:
        return "EOR";
    case BR:
        return "BR";
    case SAL:
        return "SAL";
    case SAR:
        return "SAR";
    case LDR:
        return "LDR";
    case STR:
        return "STR";
    default:
        return "UNKNOWN";
    }
}

// Function to print decoded instruction in human-readable format
void print_decoded_instruction(uint8_t opcode, uint8_t r1, uint8_t r2, int8_t immediate, int is_r_format)
{
    const char *mnemonic = get_opcode_mnemonic(opcode);

    printf("  Decoded: %s ", mnemonic);

    // All instructions have at least one register
    printf("R%u", r1);

    if (is_r_format)
    {
        // R-format instructions have two registers
        printf(" R%u", r2);
    }
    else
    {
        // I-format instructions have an immediate
        printf(" %d", immediate);
    }

    printf("\n");
}
