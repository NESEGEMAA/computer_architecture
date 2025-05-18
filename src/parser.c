/**
 * parser.c
 * Implementation of the parser module for Package 3: Double Big Harvard combo large arithmetic shifts
 */

#include "../include/parser.h"

// Helper function to get the opcode enum value from the mnemonic
Opcode get_opcode_from_mnemonic(const char* mnemonic) {
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
    exit(1);
}

// Helper function to get the instruction type from the opcode
InstructionType get_instruction_type(Opcode opcode) {
    switch(opcode) {
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
            exit(1);
    }
}

// Helper function to extract register number from a string like "R12"
uint8_t extract_register_number(const char* reg_str) {
    // Skip the 'R' character
    if (reg_str[0] != 'R') {
        fprintf(stderr, "Invalid register format: %s\n", reg_str);
        exit(1);
    }
    
    return (uint8_t)atoi(reg_str + 1);
}

// Helper function to extract immediate value from a string
int16_t extract_immediate(const char* imm_str) {
    return (int16_t)atoi(imm_str);
}

// Helper function to clean a string by removing leading/trailing spaces and comments
void clean_line(char* line) {
    // Remove comments (anything after ';')
    char* comment = strchr(line, ';');
    if (comment) {
        *comment = '\0';
    }
    
    // Remove trailing whitespace
    size_t len = strlen(line);
    while (len > 0 && (line[len-1] == ' ' || line[len-1] == '\t' || line[len-1] == '\n' || line[len-1] == '\r')) {
        line[--len] = '\0';
    }
}

// Helper function to convert instruction to 16-bit binary representation
uint16_t instruction_to_binary(const Instruction* instr) {
    uint16_t binary = 0;
    
    // Set the opcode (4 bits)
    binary |= (instr->opcode & 0xF) << 12;
    
    if (instr->type == R_TYPE) {
        // Format: OOOO RRRR RRRR 0000
        binary |= (instr->r1 & 0x3F) << 6;  // R1 (6 bits)
        binary |= (instr->r2 & 0x3F);       // R2 (6 bits)
    } else { // I_TYPE
        // Format: OOOO RRRR IIII IIII
        binary |= (instr->r1 & 0x3F) << 6;  // R1 (6 bits)
        binary |= (instr->immediate & 0x3F); // Immediate (6 bits)
    }
    
    return binary;
}

Instruction* parse_assembly_file(const char* filename, int* instr_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        exit(1);
    }
    
    // First pass: count the number of instructions
    int count = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), file)) {
        clean_line(line);
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }
        
        count++;
    }
    
    *instr_count = count;
    
    // Allocate memory for instructions
    Instruction* instructions = (Instruction*)malloc(count * sizeof(Instruction));
    if (!instructions) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }
    
    // Rewind the file for second pass
    rewind(file);
    
    // Second pass: parse instructions
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < count) {
        clean_line(line);
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }
        
        // Parse the instruction
        char mnemonic[10];
        char operands[50];
        
        // Split line into mnemonic and operands
        char* space = strchr(line, ' ');
        if (!space) {
            fprintf(stderr, "Invalid instruction format: %s\n", line);
            free(instructions);
            fclose(file);
            exit(1);
        }
        
        // Extract mnemonic
        size_t mnemonic_len = space - line;
        strncpy(mnemonic, line, mnemonic_len);
        mnemonic[mnemonic_len] = '\0';
        
        // Extract operands
        strcpy(operands, space + 1);
        
        // Get the opcode
        Opcode opcode = get_opcode_from_mnemonic(mnemonic);
        InstructionType type = get_instruction_type(opcode);
        
        // Parse operands based on instruction type
        instructions[i].opcode = opcode;
        instructions[i].type = type;
        
        // Parse operands based on format - first try commas as separators
        char* operand_list[3] = {NULL, NULL, NULL};
        int j = 0;
        
        // First check if we have commas in the operands
        if (strchr(operands, ',') != NULL) {
            // Tokenize operands by commas
            char* token = strtok(operands, ",");
            
            while (token && j < 3) {
                // Remove leading/trailing spaces
                while (*token == ' ') token++;
                operand_list[j++] = token;
                token = strtok(NULL, ",");
            }
        } else {
            // If no commas, tokenize by spaces
            char* token = strtok(operands, " \t");
            
            while (token && j < 3) {
                operand_list[j++] = token;
                token = strtok(NULL, " \t");
            }
        }
        
        if (type == R_TYPE) {
            // R-type: parse two registers
            if (!operand_list[0] || !operand_list[1]) {
                fprintf(stderr, "Invalid R-type instruction format: %s\n", line);
                free(instructions);
                fclose(file);
                exit(1);
            }
            
            instructions[i].r1 = extract_register_number(operand_list[0]);
            instructions[i].r2 = extract_register_number(operand_list[1]);
            instructions[i].immediate = 0;  // No immediate for R-type
        } else { // I_TYPE
            // I-type: parse one register and one immediate
            if (!operand_list[0] || !operand_list[1]) {
                fprintf(stderr, "Invalid I-type instruction format: %s\n", line);
                free(instructions);
                fclose(file);
                exit(1);
            }
            
            instructions[i].r1 = extract_register_number(operand_list[0]);
            instructions[i].r2 = 0;  // R2 is not used in I-type
            instructions[i].immediate = extract_immediate(operand_list[1]);
        }
        
        // Convert instruction to binary
        instructions[i].binary = instruction_to_binary(&instructions[i]);
        
        i++;
    }
    
    fclose(file);
    return instructions;
}

void print_instruction(const Instruction* instr) {
    // Print the instruction in human-readable format
    const char* type_str = (instr->type == R_TYPE) ? "R" : "I";
    const char* opcode_str[] = {"ADD", "SUB", "MUL", "MOVI", "BEQZ", "ANDI", "EOR", "BR", "SAL", "SAR", "LDR", "STR"};
    
    printf("Opcode: %s (%d), Type: %s, ", opcode_str[instr->opcode], instr->opcode, type_str);
    
    if (instr->type == R_TYPE) {
        printf("R1: R%d, R2: R%d", instr->r1, instr->r2);
    } else { // I_TYPE
        printf("R1: R%d, Immediate: %d", instr->r1, instr->immediate);
    }
    
    printf(", Binary: 0x%04X\n", instr->binary);
}

void free_instructions(Instruction* instructions) {
    free(instructions);
}