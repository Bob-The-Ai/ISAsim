#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define Iopc inst.ienc.opcode
#define Iop1 reg[inst.ienc.op1]
#define Iop2 reg[inst.ienc.op2]
#define Iimm inst.ienc.imm

#define Ropc inst.renc.opcode
#define Rop1 reg[inst.renc.op1]
#define Rop2 reg[inst.renc.op2]
#define Rop3 reg[inst.renc.op3]
#define Rshf inst.renc.shift
#define Rflg inst.renc.flags

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

int fetch(int pc, char* code) {
    char buf[4];

    memcpy(&buf, &code[pc], 4);

    return *((int*)&buf);
}

instruction decode(int raw) {
    instruction res;
    char type_LUT[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1};

    char opcode = raw >> 26;
    char type = type_LUT[opcode];
    res.type = type;
    if (type) {
        res.ienc.opcode = opcode;
        res.ienc.op1 = (raw >> 21) & 0x1f;
        res.ienc.op2 = (raw >> 16) & 0x1f;
        res.ienc.imm = raw & 0xffff;

    } else {
        res.renc.opcode = opcode;
        res.renc.op1 = (raw >> 21) & 0x1f;
        res.renc.op2 = (raw >> 16) & 0x1f;
        res.renc.op3 = (raw >> 11) & 0x1f;
        res.renc.shift = (raw >> 6) & 0x1f;
        res.renc.flags = raw & 0b111111;
    }
    return res;
}

int execute(instruction inst, int* reg, int* pc, char* mem, int* ic) {
    int should_exit = 0;
    *ic += 1;
    if (inst.type) {
        // only the ldi instruction is here but we have to futureproof.
        switch (inst.ienc.opcode) {
        case 0:
            // printf("ldi\n");
            reg[inst.ienc.op1] = inst.ienc.imm;
            *pc += 4;
            // Iop1 = Iimm;
            break;
        case 9:
            // printf("%d\n", (int16_t)inst.ienc.imm << 2);
            if (inst.ienc.op2 == 0) *pc += ((int16_t)inst.ienc.imm) << 2;
            else if ((reg[inst.ienc.op1] >> inst.ienc.op2) & 0b1) *pc += ((int16_t)inst.ienc.imm) << 2;
            else *pc += 4;
            break;
        }
    } else {
        int flg_acc = 0;
        switch (inst.renc.opcode) {
        case 1:
            // reg[inst.renc.op1] = mem[reg[inst.renc.op2]];
            memcpy((char*)&reg[inst.renc.op1], &mem[reg[inst.renc.op2]], 4);
            *pc += 4;
            break;
        case 2:
            // printf("stw\n");
            // mem[reg[inst.renc.op1]] = reg[inst.renc.op2];
            memcpy(&mem[reg[inst.renc.op1]], (char*)&reg[inst.renc.op2], 4);
            printf("0x%08x\n", reg[inst.renc.op2]);
            *pc += 4;
            break;
        case 3:
            // printf("add\n");
            reg[inst.renc.op1] = reg[inst.renc.op2] + reg[inst.renc.op3];
            // Rop1 = Rop2 + Rop3;
            *pc += 4;
            break;
        case 4:
            // printf("sll\n");
            reg[inst.renc.op1] = (reg[inst.renc.op2] << inst.renc.shift) & 0xFFFFFFFF;
            // Rop1 = (Rop2 << Rshf) & 0xFFFFFFFF;
            *pc += 4;
            break;
        case 5:
            // printf("neg\n");
            reg[inst.renc.op1] = -1 * reg[inst.renc.op2];
            // Rop1 = -1 * Rop2;
            *pc += 4;
            break;
        case 6:
            printf("hlt\nExecution ended after running %d instructions.\n", *ic);
            should_exit = 1;
            *pc += 4;
            break;
        case 7: // tst
            if (reg[inst.renc.op2] == 0) flg_acc |= 1 << 1;
            if (reg[inst.renc.op2] > 0) flg_acc |= 1 << 8;
            else flg_acc |= 1 << 9;
            reg[inst.renc.op1] = flg_acc;
            *pc += 4;
            break;
        case 8: // cmp
            if (reg[inst.renc.op2] == reg[inst.renc.op3]) flg_acc |= 1 << 2;
            if (reg[inst.renc.op2] > reg[inst.renc.op3]) flg_acc |= 1 << 3;
            if (reg[inst.renc.op2] < reg[inst.renc.op3]) flg_acc |= 1 << 4;
            *pc += 4;
            break;
        }
    }
    return should_exit;
}

int main(int argc, char** argv) {

    printf("int size: %d\n", sizeof(int));
    if (argc < 2) exit(1);

    FILE* fptr;
    int size;
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) exit(1);

    fseek(fptr, 0, SEEK_END);
    size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char* code = (char*)malloc(size);

    fread(code, size, 1, fptr);

    fclose(fptr);

    printf("Loaded file!\nSize: %d\n\n", size);
    char* memory = (char*)malloc(0xffff); // We currently only allocate 65kb of memory. this might cause some programs to fail, but you can easily increase this.

    int pc = 0;
    int registers[32] = { 0 }; // s0-sf, t0-tf + fr (flag register)
    int ic = 0;

    int want_to_die = 0;
    while (want_to_die == 0) {
        int raw = fetch(pc, code);
        // printf("0x%08x\n", raw);
        instruction inst = decode(raw);
        want_to_die = execute(inst, registers, &pc, memory, &ic);
    }

    free(code);
    return 0;
}
