#include "../include/globals.h"
#include "../include/parser.h"
#include <ctype.h> // for isspace()
#include <stdio.h> // for printf

// Helper function to get the opcode enum value from the mnemonic
static Opcode get_opcode_from_mnemonic(const char *mnemonic)
{
    Opcode opcode = get_instruction_enum(mnemonic);

    if (opcode == INVALID_INSTRUCTION)
    {
        fprintf(stderr, "[PARSER] Unknown mnemonic: %s\n", mnemonic);
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "[PARSER]   Opcode: %d\n", opcode);
    return opcode;
}

// Helper function to get the instruction type from the opcode
// static InstructionType get_instruction_type(Opcode opcode)
// {
//     switch (opcode)
//     {
//     case ADD:
//     case SUB:
//     case MUL:
//     case EOR:
//     case BR:
//         return R_TYPE;
//     case MOVI:
//     case BEQZ:
//     case ANDI:
//     case SAL:
//     case SAR:
//     case LDR:
//     case STR:
//         return I_TYPE;
//     default:
//         fprintf(stderr, "[PARSER] Unknown opcode: %d\n", opcode);
//         exit(EXIT_FAILURE);
//     }
// }

// Helper function to extract register number from a string like "R12"
static uint8_t extract_register_number_or_immediate(const char *reg_str)
{
    if (reg_str[0] != 'R')
    {
        // If not "R", treat as immediate (6-bit)
        int16_t value = atoi(reg_str);
        // Ensure the value fits in 6 bits (0-63)
        if (value < 0 || value > 63)
        {
            fprintf(stderr, "[PARSER] Error: Immediate value %d out of range (0-63) for 6-bit field\n", value);
            return value & 0x3F; // Return truncated value (6 bits only)
        }
        return (uint8_t)(value & 0x3F); // Ensure only 6 bits are used
    }
    return (uint8_t)atoi(reg_str + 1);
}

// Helper function to extract immediate value from a string
static int16_t extract_immediate(const char *imm_str)
{
    int16_t value = (int16_t)atoi(imm_str);
    // We're keeping this as a 16-bit value, but displaying a warning if it exceeds 6-bit range
    if (value < 0 || value > 63)
    {
        fprintf(stderr, "[PARSER] Warning: Immediate value %d exceeds 6-bit range (0-63)\n", value);
    }
    return value;
}

// Helper function to clean a string by removing leading/trailing spaces and comments
static void clean_line(char *line)
{
    // Remove comments (anything after ';')
    char *comment = strchr(line, ';');
    if (comment)
        *comment = '\0';

    // Remove trailing whitespace
    size_t len = strlen(line);
    while (len > 0 && isspace(line[len - 1]))
        line[--len] = '\0';
}

// Helper function to convert instruction to 16-bit binary representation
static uint16_t instruction_to_binary(const InstructionParser *instr)
{
    uint16_t binary = 0;
    binary |= (instr->opcode & 0xF) << 12;    // Opcode (4 bits)
    binary |= (instr->operand_1 & 0x3F) << 6; // R1 (6 bits)
    binary |= (instr->operand_2 & 0x3F);      // R2 (6 bits) or immediate (6 bits)

    return binary;
}

// Helper function to parse a single line into an instruction
static instruction_word_t parse_instruction_line(const char *line)
{
    InstructionParser instr;
    char mnemonic[10];
    char operands[50];

    printf("[PARSER]   Parsing line: \"%s\"\n", line); // Debug: show input line

    // Split line into mnemonic and operands
    char *space = strchr(line, ' ');
    if (!space)
    {
        fprintf(stderr, "[PARSER] Invalid instruction format: %s\n", line);
        exit(EXIT_FAILURE);
    }

    // Extract mnemonic
    size_t mnemonic_len = space - line;
    strncpy(mnemonic, line, mnemonic_len);
    mnemonic[mnemonic_len] = '\0';

    // Extract operands
    strcpy(operands, space + 1);

    printf("[PARSER]   Mnemonic: %s, Operands: %s\n", mnemonic, operands); // Debug

    // Get the opcode
    instr.opcode = get_opcode_from_mnemonic(mnemonic);

    // Parse operands
    char *operand_list[2] = {NULL, NULL};
    int j = 0;

    if (strchr(operands, ',') != NULL)
    {
        char *token = strtok(operands, ",");
        while (token && j < 3)
        {
            while (*token == ' ')
                token++;
            operand_list[j++] = token;
            token = strtok(NULL, ",");
        }
    }
    else
    {
        char *token = strtok(operands, " \t");
        while (token && j < 3)
        {
            operand_list[j++] = token;
            token = strtok(NULL, " \t");
        }
    }

    // if (instr.type == R_TYPE)
    // {
    instr.operand_1 = extract_register_number_or_immediate(operand_list[0]);
    instr.operand_2 = extract_register_number_or_immediate(operand_list[1]);
    //     instr.immediate = 0;

    //     printf("[PARSER]   Parsed R-type: R1 = R%d, R2 = R%d\n", instr.r1, instr.r2); // Debug
    // }
    // else
    // {
    //     instr.r1 = extract_register_number(operand_list[0]);
    //     instr.r2 = 0;
    //     instr.immediate = extract_immediate(operand_list[1]);

    //     printf("[PARSER]   Parsed I-type: R1 = R%d, Immediate = %d\n", instr.r1, instr.immediate); // Debug
    // }

    instruction_word_t binary = instruction_to_binary(&instr);
    printf("[PARSER]   Binary: 0x%04X\n\n", binary); // Debug: show binary representation

    return binary;
}

uint16_t parse_and_load_assembly_file(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        fprintf(stderr, "[PARSER] Failed to open file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    // init_instr_memory(); // Initialize instruction memory ------ init in main.c
    uint16_t address = 0;
    char line[256];

    printf("[PARSER]   Loading assembly from: %s\n", file_path); // Debug

    while (fgets(line, sizeof(line), file) && address < INSTR_MEMORY_SIZE)
    {
        clean_line(line);
        if (strlen(line) == 0)
            continue; // Skip empty lines

        instruction_word_t current_instruction = parse_instruction_line(line);
        write_instruction(address++, current_instruction);
    }

    fclose(file);
    printf("[PARSER] Finished loading %d instructions.\n", address); // Debug summary
    return address;
}

void print_instruction_binary(const instruction_word_t binary)
{
    // const char *type_str = (instr->type == R_TYPE) ? "R" : "I";
    // const char *opcode_str[] = {
    //     "ADD", "SUB", "MUL", "MOVI", "BEQZ",
    //     "ANDI", "EOR", "BR", "SAL", "SAR", "LDR", "STR"};
    // printf("[PARSER] Opcode: %s (%d), Type: %s, ",
    //        opcode_str[instr->opcode], instr->opcode, type_str);
    // if (instr->type == R_TYPE)
    // {
    //     printf("[PARSER] R1: R%d, R2: R%d", instr->r1, instr->r2);
    // }
    // else
    // {
    //     printf("[PARSER] R1: R%d, Immediate: %d", instr->r1, instr->immediate);
    // }

    printf("[PARSER] , Binary: 0x%04X\n", binary);
}