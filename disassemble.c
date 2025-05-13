#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "ISA.h"

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

void inst_to_line(instruction inst, FILE* fptr) {
    if (inst.type) { // I type
        fprintf(fptr, "%s %s, %s, %d\n",
                opcodes[inst.ienc.opcode],
                registers[inst.ienc.op1],
                registers[inst.ienc.op2], 
                (int16_t)inst.ienc.imm
                );
    } else { // R type
        fprintf(fptr, "%s %s, %s, %s, %u, %x\n", 
                opcodes[inst.renc.opcode], 
                registers[inst.renc.op1], 
                registers[inst.renc.op2], 
                registers[inst.renc.op3], 
                inst.renc.shift, 
                inst.renc.flags
                );
    }
}

int main(int argc, char** argv) {

    FILE* outf;

    printf("int size: %d\n", sizeof(int));
    if (argc < 2) exit(1);
    else if (argc < 3) outf = stdout;
    else outf = fopen(argv[2], "w");

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

    int want_to_die = 0;
    for (int i=0; i < size; i+=4) {
        int raw = fetch(i, code);
        // printf("0x%08x\n", raw);
        instruction inst = decode(raw);
        inst_to_line(inst, outf);
    }

    fclose(outf);

    free(code);
    return 0;
}
