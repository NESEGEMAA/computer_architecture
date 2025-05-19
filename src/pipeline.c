#include "pipeline.h"

int cycle = 0; // Cycle counter
int stop = 0;  // Stop flag

void fetch_stage();
void execute_stage();
void opcode_func(Opcode opcode);

void pipeline_cycle()
{
    printf("\nCycle %d\n", cycle);
    fetch_stage();

    if (PC > 1)
    {
        if (stop >= 2)
            printf("Decode Stage: Stopped\n");
        else
            decode_stage(); // since fetch increments PC by 1 so decode should be skipped for first cycle
    }

    if (PC > 2)
    {
        if (stop >= 3)
            printf("Execute Stage: Stopped\n");
        else
        execute_stage();
    }

    cycle++;
}

void fetch_stage()
{
    IF_ID if_id = {0}; // Instruction Fetch to Decode stage

    // Store the PC value at the start of fetch
    instruction_word_t fetch_pc = PC;
    
    // Fetch stage
    instruction_word_t instruction = read_instruction(PC);
    if (instruction == UNDEFINED_INT16)
    {
        stop++;
        printf("Fetch Stage: Stopped\n");
        return;
    }

    printf("Fetch Stage: PC: %d, Instruction: 0x%04X\n", PC, instruction);

    if_id.instr = instruction;
    if_id.pc = ++PC;

    // Show the input values (PC) and output (the instruction and next PC)
    printf("  Input: PC = %d\n", fetch_pc);
    printf("  Output: Fetched instruction = 0x%04X, Next PC = %d\n", instruction, PC);

    enqueue_if_id(&if_id_queue, &if_id);
    printf("To be decoded ");
    print_queue(&if_id_queue); // Print the queue after processing
}

void execute_stage()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    
    // Print the instruction entering the execute stage
    printf("Execute Stage: Instruction: 0x%04X, Opcode: %s, PC: %d\n", 
           id_ex.instruction, 
           get_opcode_mnemonic(id_ex.opcode),
           id_ex.pc);

    // Print the values entering the stage
    printf("  Input Values: ");
    if (isit_r_format(id_ex.opcode)) {
        printf("R%d = %d, R%d = %d\n", id_ex.r1, id_ex.r1_value, id_ex.r2, id_ex.r2_value);
    } else {
        printf("R%d = %d, Immediate = %d\n", id_ex.r1, id_ex.r1_value, id_ex.immediate);
    }
    
    // Store register values before execution for comparison
    data_word_t old_register_values[REG_COUNT];
    for (int i = 0; i < REG_COUNT; i++) {
        old_register_values[i] = read_register(i);
    }
    
    // Store the SREG value before execution
    data_word_t old_SREG = SREG;
    
    // Store PC before execution
    instruction_word_t old_PC = PC;
    
    // Execute the instruction
    opcode_func(id_ex.opcode);

    // Check for changes in registers
    for (int i = 0; i < REG_COUNT; i++) {
        if (read_register(i) != old_register_values[i]) {
            printf("  Register Change in Execute Stage: R%d changed from %d to %d\n", 
                   i, old_register_values[i], read_register(i));
        }
    }
    
    // Check for changes in SREG
    if (SREG != old_SREG) {
        printf("  SREG Change in Execute Stage: Changed from 0x%02X to 0x%02X\n", 
               old_SREG, SREG);
    }
    
    // Check for changes in PC (for branch instructions)
    if (PC != old_PC) {
        printf("  PC Change in Execute Stage: Changed from %d to %d\n", 
               old_PC, PC);
    }

    // Dequeue from Decode to Execute stage
    dequeue_id_ex(&id_ex_queue);

    if (isEmpty(&id_ex_queue))
    {
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