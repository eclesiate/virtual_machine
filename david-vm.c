#include "david-vm.h"
int main () {
    printf("hello world!");
    return 0;

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
            vm->stack[vm->stack_sz--];
            break;
        }
        case INSTR_SUM: {
            if (vm->stack_sz < 2) {
                return TRAP_STACK_UNDERFLOW;
            }
            vm->stack[vm->stack_sz - 2] += vm->stack[--vm->stack_sz]; // ?
            break;
        }
        default: 

    }
}