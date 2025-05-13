#pragma once
#include <stdint.h>

const char* opcodes[10] = {
    "ldi",
    "ldw",
    "stw",
    "add",
    "sll",
    "neg",
    "hlt",
    "tst",
    "cmp",
    "brc"
};

const char* registers[32] = {
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "sa", "sb", "sc", "sd", "se", "sf",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "ta", "tb", "tc", "td", "te", "tf"
};

typedef struct {
    uint8_t opcode;
    uint8_t op1;
    uint8_t op2;
    uint8_t op3;
    uint8_t shift;
    uint8_t flags;
} RType;

typedef struct {
    uint8_t opcode;
    uint8_t op1;
    uint8_t op2;
    uint16_t imm;
} IType;

typedef struct {
    int type;
    RType renc;
    IType ienc;
} instruction;


