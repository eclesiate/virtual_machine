#include "david-vm.h"

Vm vm = {0};

Instr program [] = {
    GIVE_INSTR_PUSH(0),
    GIVE_INSTR_PUSH(1),
    GIVE_INSTR_SUM,
    GIVE_INSTR_JMP(1),
    // GIVE_INSTR_PUSH(69),
    // GIVE_INSTR_PUSH(420),
    // GIVE_INSTR_SUM,
    // GIVE_INSTR_PUSH(10),
    // GIVE_INSTR_MINUS,
    // GIVE_INSTR_PUSH(3),
    // GIVE_INSTR_MULT,
    // GIVE_INSTR_PUSH(5), 
    // GIVE_INSTR_DIV,
};

Trap execute_instr(Vm* vm) {
    if (vm->PC < 0 || vm->PC >= vm->program_sz) {
        return TRAP_ILLEGAL_INSTR_ADDRESS;
    }
    Instr instr = vm->program[vm->PC];

    switch(instr.type) {
        case INSTR_PUSH: {
            if (vm->stack_sz >= VM_STACK_CAP) {
                return TRAP_STACK_OVERFLOW;
            }
            vm->stack[vm->stack_sz++] = instr.operand;
            vm->PC += 1;
            break;
        }
        case INSTR_POP: {
            if ((vm->stack_sz) <= 0) {
                return TRAP_STACK_UNDERFLOW;
            }
            --vm->stack_sz;
            vm->PC += 1;
            break;
        }
        case INSTR_SUM: { 
            if (vm->stack_sz < 2) {
                return TRAP_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_sz - 2] += vm->stack[vm->stack_sz - 1]; // ! don't do pre decrement on RHS if LHS is - 2
            vm->stack_sz -= 1;
            vm->PC += 1;
            break;
        }
        case INSTR_MINUS: { 
            if (vm->stack_sz < 2) {
                return TRAP_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_sz - 2] -= vm->stack[vm->stack_sz - 1];
            vm->stack_sz -= 1;
            vm->PC += 1;
            break;
        }
        case INSTR_MULT: { 
            if (vm->stack_sz < 2) {
                return TRAP_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_sz - 2] *= vm->stack[vm->stack_sz - 1];
            vm->stack_sz -= 1;
            vm->PC += 1;
            break;
        }
        case INSTR_DIV: { 
            if (vm->stack_sz < 2) {
                return TRAP_STACK_UNDERFLOW;
            }
            if (vm->stack[vm->stack_sz - 1] == 0) {
                return TRAP_DIV_BY_ZERO;
            }
            vm->stack[vm->stack_sz - 2] /= vm->stack[vm->stack_sz - 1];
            vm->stack_sz -= 1;
            vm->PC += 1;
            break;
        }
        case INSTR_JMP: { 
            vm->PC = instr.operand;
            break;
        }
        case INSTR_HALT: { 
            if (vm->running) {
                vm->running = false;
            }
            if (vm->stack[vm->stack_sz - 1] == 0) {
                return TRAP_DIV_BY_ZERO;
            }
            vm->stack[vm->stack_sz - 2] /= vm->stack[vm->stack_sz - 1];
            vm->stack_sz -= 1;
            vm->PC += 1;
            break;
        }
        

        default: {
            return TRAP_ILLEGAL_INSTR;
        }
    }
    return TRAP_OK;
}

void dump_vm_stack(FILE* stream, const Vm* vm) {
    if (vm->stack_sz <= 0) {
        fprintf(stream, "Stack is empty\n");
        return;
    }
    printf("Stack:\n");
    for (Word i = 0; i < vm->stack_sz; ++i) {
        fprintf(stream, "  %ld\n", vm->stack[i]);
    }
}

void vm_load_program_from_memory(Vm* vm, Instr* program, size_t program_sz) {
    assert(vm->program_sz < PROGRAM_CAP && program_sz < PROGRAM_CAP);
    memcpy(vm->program, program, sizeof(program[0]) * program_sz);
    vm->program_sz = program_sz;
}

const char* trap_as_str(Trap trap) {
    switch(trap) {
        case TRAP_OK:
            return "TRAP_OK";
        case TRAP_ILLEGAL_INSTR: 
            return "TRAP_ILLEGAL_INSTR";
        case TRAP_STACK_OVERFLOW:
            return "TRAP_STACK_OVERFLOW";
        case TRAP_STACK_UNDERFLOW:
            return "TRAP_STACK_UNDERFLOW";
        case TRAP_DIV_BY_ZERO:
            return "TRAP_DIV_BY_ZERO";
        case TRAP_ILLEGAL_INSTR_ADDRESS:
            return "TRAP_ILLEGAL_INSTR_ADDRESS";
        default:
            assert(0 && "trap_as_str: Unreachable"); //@cite gpt, to force crash with err msg
    }
}

const char* instr_as_str(Instr_Type instr) {
    switch(instr) {
        case INSTR_PUSH:
            return "INSTR_PUSH";
        case INSTR_POP: 
            return "INSTR_POP";
        case INSTR_SUM:
            return "INSTR_SUM";
        case INSTR_MINUS:
            return "INSTR_MINUS";
        case INSTR_DIV:
            return "INSTR_DIV";
        case INSTR_MULT:
            return "INSTR_MULT";
        case INSTR_JMP:
            return "INSTR_JMP";
        case INSTR_HALT:
            return "INSTR_HALT";
        default:
            assert(0 && "instr_as_str: Unreachable"); //@cite gpt, to force crash with err msg
    }
}

int main () {

    vm_load_program_from_memory(&vm, program, ARRAY_SIZE(program));

    dump_vm_stack(stdout, &vm);
    while(vm.running) {
        fprintf(stdout, "%s\n", instr_as_str(vm.program[vm.PC].type));
        Trap trap = execute_instr(&vm);
        if (trap != TRAP_OK) {
            fprintf(stderr, "Trap Occured: %s\n", trap_as_str(trap));
            dump_vm_stack(stderr, &vm);
            exit(1);
        }
    }
    dump_vm_stack(stdout, &vm);
    return 0;
}