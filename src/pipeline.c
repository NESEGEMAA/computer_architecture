#include "pipeline.h"

int cycle = 0; // Cycle counter
// int stop = 0; // Stop flag

void fetch_stage();
void execute_stage();
void opcode_func(Opcode opcode);

void pipeline_cycle()
{
    
    fetch_stage();

    if (PC > 0) decode_stage();

    if (PC > 1) execute_stage();

    cycle++;
}

void fetch_stage()
{
    IF_ID if_id = {0}; // Instruction Fetch to Decode stage

    // Fetch stage
    instruction_word_t instruction = read_instruction(PC);

    printf("Fetch Stage: PC: %d, Instruction: 0x%04X\n", PC, instruction);

    if (instruction == UNDEFINED_INT16) {
        return;
    }

    if_id.instr = instruction;
    if_id.pc = ++PC;

    enqueue_if_id(&if_id_queue, &if_id);
}

void execute_stage()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    opcode_func(id_ex.opcode);

    // Dequeue from Decode to Execute stage
    dequeue_id_ex(&id_ex_queue);

    if (isEmpty(&id_ex_queue)) {
        sys_call = 0; // Stop execution if queue is empty
        return;
    }
}

void opcode_func(Opcode opcode)
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    switch (opcode)
    {
        case ADD:
            _ADD();
            break;
        case SUB:
            _SUB();
            break;
        case MUL:
            _MUL();
            break;
        case MOVI:
            _MOVI();
            break;
        case BEQZ:
            _BEQZ();
            break;
        case ANDI:
            _ANDI();
            break;
        case EOR:
            _EOR();
            break;
        case BR:
            _BR();
            break;
        case SAL:
            _SAL();
            break;
        case SAR:
            _SAR();
            break;
        case LDR:
            _LDR();
            break;
        case STR:
            _STR();
            break;
        default:
            fprintf(stderr, "Error: Unknown opcode %d\n", id_ex.opcode);
    }
}