// You can use the created enum in instruction_map.h to represent the instruction types.

// Instruction instr_type = get_instruction_enum(operation_name);
// switch (instr_type) {
//     case ADD:
//         _ADD(rd, rs);
//         break;
//     case SUB:
//         _SUB(rd, rs);
//         break;
//     // ... other cases
//     default:
//         fprintf(stderr, "Invalid instruction at PC=%u\n", PC);
// }