
#include "../include/parser.h"
#include <ctype.h>  // for isspace()

// Helper function to get the opcode enum value from the mnemonic
static Opcode get_opcode_from_mnemonic(const char *mnemonic)
{
    if (strcmp(mnemonic, "ADD") == 0) return ADD;
    if (strcmp(mnemonic, "SUB") == 0) return SUB;
    if (strcmp(mnemonic, "MUL") == 0) return MUL;
    if (strcmp(mnemonic, "MOVI") == 0) return MOVI;
    if (strcmp(mnemonic, "BEQZ") == 0) return BEQZ;
    if (strcmp(mnemonic, "ANDI") == 0) return ANDI;
    if (strcmp(mnemonic, "EOR") == 0) return EOR;
    if (strcmp(mnemonic, "BR") == 0) return BR;
    if (strcmp(mnemonic, "SAL") == 0) return SAL;
    if (strcmp(mnemonic, "SAR") == 0) return SAR;
    if (strcmp(mnemonic, "LDR") == 0) return LDR;
    if (strcmp(mnemonic, "STR") == 0) return STR;

    fprintf(stderr, "Unknown mnemonic: %s\n", mnemonic);
    exit(EXIT_FAILURE);
}

// Helper function to get the instruction type from the opcode
static InstructionType get_instruction_type(Opcode opcode)
{
    switch (opcode)
    {
    case ADD:
    case SUB:
    case MUL:
    case EOR:
    case BR:
        return R_TYPE;
    case MOVI:
    case BEQZ:
    case ANDI:
    case SAL:
    case SAR:
    case LDR:
    case STR:
        return I_TYPE;
    default:
        fprintf(stderr, "Unknown opcode: %d\n", opcode);
        exit(EXIT_FAILURE);
    }
}

// Helper function to extract register number from a string like "R12"
static uint8_t extract_register_number(const char *reg_str)
{
    if (reg_str[0] != 'R')
    {
        fprintf(stderr, "Invalid register format: %s\n", reg_str);
        exit(EXIT_FAILURE);
    }
    return (uint8_t)atoi(reg_str + 1);
}

// Helper function to extract immediate value from a string
static int16_t extract_immediate(const char *imm_str)
{
    return (int16_t)atoi(imm_str);
}

// Helper function to clean a string by removing leading/trailing spaces and comments
static void clean_line(char *line)
{
    // Remove comments (anything after ';')
    char *comment = strchr(line, ';');
    if (comment) *comment = '\0';

    // Remove trailing whitespace
    size_t len = strlen(line);
    while (len > 0 && isspace(line[len - 1]))
        line[--len] = '\0';
}

// Helper function to convert instruction to 16-bit binary representation
static uint16_t instruction_to_binary(const InstructionParser *instr)
{
    uint16_t binary = 0;
    binary |= (instr->opcode & 0xF) << 12; // Opcode (4 bits)

    if (instr->type == R_TYPE)
    {
        binary |= (instr->r1 & 0x3F) << 6; // R1 (6 bits)
        binary |= (instr->r2 & 0x3F);      // R2 (6 bits)
    }
    else
    { // I_TYPE
        binary |= (instr->r1 & 0x3F) << 6;   // R1 (6 bits)
        binary |= (instr->immediate & 0xFF); // Immediate (8 bits)
    }
    return binary;
}

// Helper function to parse a single line into an instruction
static InstructionParser parse_instruction_line(const char *line)
{
    InstructionParser instr;
    char mnemonic[10];
    char operands[50];

    // Split line into mnemonic and operands
    char *space = strchr(line, ' ');
    if (!space)
    {
        fprintf(stderr, "Invalid instruction format: %s\n", line);
        exit(EXIT_FAILURE);
    }

    // Extract mnemonic
    size_t mnemonic_len = space - line;
    strncpy(mnemonic, line, mnemonic_len);
    mnemonic[mnemonic_len] = '\0';

    // Extract operands
    strcpy(operands, space + 1);

    // Get the opcode and type
    instr.opcode = get_opcode_from_mnemonic(mnemonic);
    instr.type = get_instruction_type(instr.opcode);

    // Parse operands
    char *operand_list[3] = {NULL, NULL, NULL};
    int j = 0;

    if (strchr(operands, ',') != NULL)
    {
        char *token = strtok(operands, ",");
        while (token && j < 3)
        {
            while (*token == ' ') token++;
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

    if (instr.type == R_TYPE)
    {
        if (!operand_list[0] || !operand_list[1])
        {
            fprintf(stderr, "Invalid R-type instruction format: %s\n", line);
            exit(EXIT_FAILURE);
        }
        instr.r1 = extract_register_number(operand_list[0]);
        instr.r2 = extract_register_number(operand_list[1]);
        instr.immediate = 0;
    }
    else
    {
        if (!operand_list[0] || !operand_list[1])
        {
            fprintf(stderr, "Invalid I-type instruction format: %s\n", line);
            exit(EXIT_FAILURE);
        }
        instr.r1 = extract_register_number(operand_list[0]);
        instr.r2 = 0;
        instr.immediate = extract_immediate(operand_list[1]);
    }

    instr.binary = instruction_to_binary(&instr);
    return instr;
}

uint16_t parse_and_load_assembly_file(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to open file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    init_instr_memory(); // Initialize instruction memory
    uint16_t address = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && address < INSTR_MEMORY_SIZE)
    {
        clean_line(line);
        if (strlen(line) == 0) continue; // Skip empty lines

        InstructionParser instr = parse_instruction_line(line);
        instr_memory[address++] = instr.binary;

        // Optional debug print
        // print_instruction(&instr);
    }

    fclose(file);
    return address; // Return number of instructions loaded
}

void print_instruction(const InstructionParser *instr)
{
    const char *type_str = (instr->type == R_TYPE) ? "R" : "I";
    const char *opcode_str[] = {
        "ADD", "SUB", "MUL", "MOVI", "BEQZ", 
        "ANDI", "EOR", "BR", "SAL", "SAR", "LDR", "STR"
    };

    printf("Opcode: %s (%d), Type: %s, ", 
           opcode_str[instr->opcode], instr->opcode, type_str);

    if (instr->type == R_TYPE)
    {
        printf("R1: R%d, R2: R%d", instr->r1, instr->r2);
    }
    else
    {
        printf("R1: R%d, Immediate: %d", instr->r1, instr->immediate);
    }

    printf(", Binary: 0x%04X\n", instr->binary);
}