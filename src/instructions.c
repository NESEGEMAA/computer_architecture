#include "instructions.h"
#include "pipeline.h"
#include "types.h"

// Helper function to update the Carry flag (C)
void update_carry_flag(int16_t result)
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

void _ADD()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    data_word_t destination = id_ex.r1_value;
    data_word_t source = id_ex.r2_value;

    // Data hazard forwarding for source operand
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r2==1)
        source = EX.result;
        if(EX.r1==1)
        destination = EX.result;
        printf("Data hazard detected in ADD instruction. Forwarding value: %d...\n", source);
        data_hazard = 0;
        data_stall = 1;
    }

    int16_t result = destination + source;
    EX.result = result;

    // Update relevant flags for ADD
    update_flags(ADD, destination, source, result);

    uint8_t rd = id_ex.r1;
    write_register(rd, (int8_t)result);

    printf("ADD: R%u = %d + %d = %d\n", rd, destination, source, result);
}

void _SUB()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue));
    data_word_t destination = id_ex.r1_value;
    data_word_t source = id_ex.r2_value;

    // Data hazard forwarding
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1)
            destination = EX.result;
        if(EX.r2==1) 
            source = EX.result;
        printf("Data hazard detected in SUB instruction. Forwarding value: %d...\n", 
               (EX.r1==1) ? destination : source);
        data_hazard = 0;
        data_stall = 1;
    }

    int16_t result = destination - source;
    EX.result = result;
    
    update_flags(SUB, destination, source, result);

    uint8_t rd = id_ex.r1;
    write_register(rd, (int8_t)result);

    printf("SUB: R%u = %d - %d = %d\n", rd, destination, source, result);
}

void _MUL()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue));
    data_word_t destination = id_ex.r1_value;
    data_word_t source = id_ex.r2_value;

    // Data hazard forwarding
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1)
            destination = EX.result;
        if(EX.r2==1) 
            source = EX.result;
        printf("Data hazard detected in MUL instruction. Forwarding value: %d...\n", 
               (EX.r1==1) ? destination : source);
        data_hazard = 0;
        data_stall = 1;
    }

    int16_t result = destination * source;
    EX.result = result;

    update_flags(MUL, destination, source, result);

    uint8_t rd = id_ex.r1;
    write_register(rd, (int8_t)result);

    printf("MUL: R%u = %d * %d = %d\n", rd, destination, source, result);
}

void _MOVI()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    uint8_t rd = id_ex.r1;
    int8_t immediate = id_ex.immediate;
    EX.result = immediate;

    // Move immediate value to register rd
    write_register(rd, immediate);

    printf("MOVI: R%u = %d\n", rd, immediate);
}

void _BEQZ()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    int8_t value = id_ex.r1_value;
    int8_t immediate = id_ex.immediate;
    
    // Add data hazard forwarding for r1
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1) {
            value = EX.result;
            printf("Data hazard detected in BEQZ instruction. Forwarding value: %d...\n", value);
        }
        data_hazard = 0;
        data_stall = 1;
    }
    
    EX.result = immediate;
    
    if (value == 0)
    {
        while (!isEmpty(&if_id_queue))
        {
            dequeue_if_id(&if_id_queue);
        }
        //flush out previous instructions
        while (!isEmpty(&id_ex_queue))
        {
            dequeue_id_ex(&id_ex_queue);
        }

        decode_stall = 1;
        execute_stall = 2;
        PC = id_ex.pc + immediate;
    }
    data_hazard = 0;

    printf("BEQZ: R%u = %d, PC = %d\n", id_ex.r1, value, PC);
}

void _ANDI()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue));
    int8_t destination = id_ex.r1_value;
    int8_t immediate = id_ex.immediate;

    // Data hazard forwarding - only check r1
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1) {
            destination = EX.result;
            printf("Data hazard detected in ANDI instruction. Forwarding value: %d...\n", destination);
        }
        data_hazard = 0;
        data_stall = 1;
    }

    int8_t result = destination & immediate;
    EX.result = result;

    // Update relevant flags for ANDI
    update_flags(ANDI, destination, immediate, result);

    uint8_t rd = id_ex.r1;
    write_register(rd, result);

    printf("ANDI: R%u = %d & %d = %d\n", rd, destination, immediate, result);
}

void _EOR()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue));
    int8_t destination = id_ex.r1_value;
    int8_t source = id_ex.r2_value;

    // Data hazard forwarding
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1)
            destination = EX.result;
        if(EX.r2==1) 
            source = EX.result;
        printf("Data hazard detected in EOR instruction. Forwarding value: %d...\n", 
               (EX.r1==id_ex.r1) ? destination : source);
        data_hazard = 0;
        data_stall = 1;
    }

    int8_t result = destination ^ source;
    EX.result = result;

    uint8_t rd = id_ex.r1;
    update_flags(EOR, destination, source, result);
    write_register(rd, result);

    printf("EOR: R%u = %d ^ %d = %d\n", rd, destination, source, result);
}

void _BR()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage

    // Branch Register - set the PC to the concatenated value of registers rd and rs
    int8_t high_byte = id_ex.r1_value;
    int8_t low_byte = id_ex.r2_value;

    // Data hazard forwarding for both registers
    if (data_hazard == 1 && data_stall == 0) {
        // Check if r1 (high byte) needs forwarding
        if(EX.r1==1) {
            high_byte = EX.result;
            printf("Data hazard detected in BR instruction. Forwarding high_byte: %d...\n", high_byte);
        }
        // Check if r2 (low byte) needs forwarding
        if(EX.r2==1) {
            low_byte = EX.result;
            printf("Data hazard detected in BR instruction. Forwarding low_byte: %d...\n", low_byte);
        }
        data_hazard = 0;
        data_stall = 1;
    }

    // Concatenate the two registers to form a 16-bit address
    uint16_t new_pc = ((uint16_t)(uint8_t)high_byte << 8) | (uint8_t)low_byte;
    EX.result = new_pc;
    
    // Flush out previous instructions
    while (!isEmpty(&if_id_queue)) {
        dequeue_if_id(&if_id_queue);
    }
    while (!isEmpty(&id_ex_queue)) {
        dequeue_id_ex(&id_ex_queue);
    }
   
    data_hazard = 0;
    decode_stall = 1;
    execute_stall = 2;

    PC = new_pc;
    
    printf("BR: PC = %d\n", PC);
}

void _SAL()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue));
    int8_t destination = id_ex.r1_value;
    int8_t immediate = id_ex.immediate;

    // Data hazard forwarding - only check r1
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1) {
            destination = EX.result;
            printf("Data hazard detected in SAL instruction. Forwarding value: %d...\n", destination);
        }
        data_hazard = 0;
        data_stall = 1;
    }

    int16_t result = destination << immediate;
    EX.result = result;

    // Update relevant flags for SAL
    update_flags(SAL, destination, immediate, result);

    uint8_t rd = id_ex.r1;
    write_register(rd, (int8_t)result);

    printf("SAL: R%u = %d << %d = %d\n", rd, destination, immediate, result);
}

void _SAR()
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue));
    int8_t destination = id_ex.r1_value;
    int8_t immediate = id_ex.immediate;

    // Data hazard forwarding - only check r1
    if (data_hazard == 1 && data_stall == 0) {
        if(EX.r1==1) {
            destination = EX.result;
            printf("Data hazard detected in SAR instruction. Forwarding value: %d...\n", destination);
        }
        data_hazard = 0;
        data_stall = 1;
    }

    int16_t result = destination >> immediate;
    EX.result = result;

    // Update relevant flags for SAR
    update_flags(SAR, destination, immediate, result);

    uint8_t rd = id_ex.r1;
    write_register(rd, (int8_t)result);

    printf("SAR: R%u = %d >> %d = %d\n", rd, destination, immediate, result);
}

void _LDR()
{ 
   
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    uint8_t address;
    int8_t value;
    uint8_t rd ;
    if (data_hazard == 1&&data_stall==0)
        {
            value = EX.result;
            printf("Data hazard detected in LDR instruction. Forwarding Memory Data : %d...\n",value);
            data_hazard = 0;
            data_stall = 1;
        }
    else{
    // Load to Register - load value from memory at address into register rd
    address = id_ex.immediate;
    value = read_data(address);
    }
    rd = id_ex.r1;
    
    // Store old register value for comparison
    int8_t old_value = id_ex.r1_value;
    EX.result = value;
    // Update the register
    write_register(rd, value);

    printf("LDR: Memory[%d] = %d -> R%u\n", address, value, rd);
    
    // Report register value change
    printf("  Register Change in Execute Stage: R%d changed from %d to %d\n", 
           rd, old_value, value);
}

void _STR()  
{
    ID_EX id_ex = *(peek_id_ex(&id_ex_queue)); // Decode to Execute stage
    uint8_t rd ;
    int8_t value ;
    int8_t address ;
    if (data_hazard==1&&data_stall==0){
        value = EX.result;
        printf("Data hazard detected in STR instruction. Forwarding Register Value : %d...\n",value);
        data_hazard = 0;
        data_stall = 1;
    }
    
    // Store from Register - store value from register rd into memory at address
   else
     value = id_ex.r1_value;
    
    rd = id_ex.r1;
    address = id_ex.immediate;
    
    // Store old memory value for comparison
    int8_t old_value = read_data(address);
    EX.result = value;
    // Update the memory
    write_data(address, value);

    // Print instruction and operands
    printf("STR: R%u = %d -> Memory[%d]\n", rd, value, address);
    
    // Report memory value change if the value actually changed
    if (old_value != value) {
        printf("  Memory Change in Execute Stage: Memory[%d] changed from %d to %d\n", 
               address, old_value, value);
    }
}