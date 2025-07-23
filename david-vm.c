#include "david-vm.h"

Vm vm = {0};

Instr instr_push(Word operand) {
    return (Instr) { // @cite compound literal from gpt
        .type = INSTR_PUSH,
        .operand = operand,
    };
}

Instr instr_sum(void) {
    return (Instr) {
        .type = INSTR_SUM,
        .operand = INT64_MAX, // TODO verify this
    };
}

Instr instr_pop(void) {
    return (Instr) {
        .type = INSTR_POP,
        .operand = INT64_MAX,
    };
}

Trap execute_instr(Vm* vm, Instr instr) {
    switch(instr.type) {
        case INSTR_PUSH: {
            if (vm->stack_sz >= VM_STACK_CAP) {
                return TRAP_STACK_OVERFLOW;
            }
            vm->stack[vm->stack_sz++] = instr.operand;
            break;
        }
        case INSTR_POP: { // TODO
            if ((vm->stack_sz) <= 0) {
                return TRAP_STACK_UNDERFLOW;
            }
            --vm->stack_sz;
            break;
        }
        case INSTR_SUM: { 
            if (vm->stack_sz < 2) {
                return TRAP_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_sz - 2] += vm->stack[vm->stack_sz - 1]; // ! don't do pre decrement on RHS if LHS is - 2
            vm->stack_sz -= 1;
            break;
        }
        default: {
            return TRAP_ILLEGAL_INSTR;
        }
    }
    return TRAP_OK;
}

void dump_vm_stack(const Vm* vm) {
    if (vm->stack_sz <= 0) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack:\n");
    for (size_t i = 0; i < vm->stack_sz; ++i) {
        printf("  %ld\n", vm->stack[i]);
    }
}

int main () {
    dump_vm_stack(&vm);
    execute_instr(&vm, instr_push(69));
    dump_vm_stack(&vm);
    execute_instr(&vm, instr_push(420));
    dump_vm_stack(&vm);
    execute_instr(&vm, instr_sum());
    dump_vm_stack(&vm);
    return 0;
}