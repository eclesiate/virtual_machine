/*
Author: David 

16b, 65kb of memory


2025-07-01
*/
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
 
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

#define $1 (uint8_t)
#define $2 (uint16_t)
#define $4 (uint32_t)
#define $8 (uint64_t)
#define $c (char*)
#define $i (int)

typedef unsigned short int reg;

typedef struct Registers Registers;
struct Registers {
    reg ax;
    reg bx; 
    reg cx;
    reg dx;
    reg sp;
    reg ip;
};

typedef struct CPU CPU;
struct CPU{
    Registers r;
};
 
typedef uint8_t Stack;

typedef uint8_t Opcode;

typedef Opcode Program;

enum e_opcode {
    MOV = 0x01,
    NOP = 0x02
}

typedef Instruction Instructon;

struct Instruction {
    Opcode o;
    Args a[];
};

typedef struct Instruction_Map IM;
struct Instruction_Map {
    Opcode o;
    uint8_t size;
}


typedef struct VM VM;
struct VM {
    CPU* cpu;
    Stack stack;
    Program* program;
};

static IM* inst_map = {
    { Opcode.MOV, 0x03}
};