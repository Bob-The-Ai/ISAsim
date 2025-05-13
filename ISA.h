#pragma once
#include <stdint.h>

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


