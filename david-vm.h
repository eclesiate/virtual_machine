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

#define PROGRAM_CAP 1024

#define ARRAY_SIZE(a) ((sizeof(a)) / (sizeof(a[0])))

typedef int64_t Word;

typedef enum {
    INSTR_PUSH,
    INSTR_POP,
    INSTR_SUM,
    INSTR_MINUS,
    INSTR_DIV,
    INSTR_MULT,
    INSTR_JMP,
    INSTR_HALT,
    INSTR_JMP_IF,

} Instr_Type;

typedef enum {
    TRAP_OK = 0,
    TRAP_STACK_OVERFLOW,
    TRAP_STACK_UNDERFLOW,
    TRAP_ILLEGAL_INSTR,
    TRAP_DIV_BY_ZERO, 
    TRAP_ILLEGAL_INSTR_ADDRESS,
} Trap;

typedef struct {
    Instr_Type type;
    Word operand;
} Instr;

typedef struct {
    Word stack[VM_STACK_CAP];
    Word stack_sz;
    Word PC;
    bool running;
    Instr program [PROGRAM_CAP];
    Word program_sz;
} Vm;

#define GIVE_INSTR_PUSH(op) {.type = INSTR_PUSH, .operand = (op)} // * macro for static initialiation of program array
#define GIVE_INSTR_POP      {.type = INSTR_POP}
#define GIVE_INSTR_SUM      {.type = INSTR_SUM}
#define GIVE_INSTR_MINUS    {.type = INSTR_MINUS}
#define GIVE_INSTR_MULT     {.type = INSTR_MULT}
#define GIVE_INSTR_DIV      {.type = INSTR_DIV}
#define GIVE_INSTR_JMP(addr){.type = INSTR_JMP, .operand = (addr)}
#define GIVE_INSTR_HALT     {.type = INSTR_HALT}

Trap execute_instr(Vm *vm);

void dump_vm_stack(FILE* stream, const Vm* vm);

void vm_load_program_from_memory(Vm* vm, Instr* program, size_t program_sz);

const char* trap_as_str(Trap trap);
const char* instr_as_str(Instr_Type instr);


#endif