/*
Author: David 

16b, 65kb of memory


2025-07-01
*/

#ifndef DAVIDVM_H
#define DAVIDVM_H

#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

#define VM_STACK_CAP 1024

typedef int64_t Word;

typedef struct {
    Word stack[VM_STACK_CAP];
    size_t stack_sz;
} Vm;

typedef enum {
    INSTR_PUSH,
    INSTR_POP,
    INSTR_SUM
} Instr_Type;

typedef enum {
    TRAP_OK = 0,
    TRAP_STACK_OVERFLOW,
    TRAP_STACK_UNDERFLOW
} Trap;

typedef struct {
    Instr_Type type;
    Word operand;
} Instr;

Trap execute_instr(Vm *vm, Instr instr);

#endif