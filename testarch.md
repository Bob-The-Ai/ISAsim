# testarch
---
A simple ISA, designed to be simulated by simulators that are still early on in development.
It provides a simple and easily attainable goal for the simulator programmers.


# Key points:
1. it is pipelined with a simple 5 stage pipeline
2. its in the RISC style, with only the very basics of instructions
3. partially MIPS like


# Instructions:
1. ldi: load immediate
2. ldw: load word
3. stw: store word
4. add: adds 2 registers
5. sll: left logical shift
6. neg: negates a register
7. hlt: ends the simulation
8. tst: tests a register by itself and sets the flags
9. cmp: compares 2 registers and sets the flags
10. brc: branches based on flags

# Registers:
Offers 32 registers of word size.
They are named s0-sf and t0-tf.
s0 is also known as sp (stack pointer).
t0 is also known as ra (return address).


# Flags:
Any register can be used to store flags. Each flag is one bit long.
The tst, cmp and brc instructions make use of them.
Those flags are:
1. false (always 0)
2. zero
3. equal
4. greater
5. lesser
6. greater equal
7. lesser equal
8. positive
9. negative


# Instruction encodings:
There are two types of encodings, R type and I type.
The instructions are encoded into 32 bits.
xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx

The 6 MSBs are reserved for the opcode, while the remaining 26 bits are for the various operands and flags.

The R type, needs to store register operands. There is no instruction requiring more than 3 of those, so we can only reserve 15 bits for them.
The rest of the 11 bits can be either as a large immediate which is counter productive, or we can add another 5 bit field for a shiftamount immediate and another 6 bits for flags.
We now end up with the following R type encoding:
opcode   op1   op2   op3     shift   flags
xxxxxx | xxxxx xxxxx xxxxx | xxxxx | xxxxxx

The I type, needs a large immediate, lets say 16 bits. 32 - 6 (for the opcode) - 16 = 10. In these 10 bits we can fit another 2 register (5 bit) fields suspiciously well.
The final I type encoding is as follows:
opcode   op1   op2     imm
xxxxxx | xxxxx xxxxx | xxxxxxxxxxxxxxxx


# Breakdown of the instructions:
1. ldi op1, imm
> I Type
> opcode: 0
> op1 -> destination, imm -> source
> function: op1 = imm

2. ldw op1, op2
> R Type
> opcode: 1
> op1 -> destination, op2 -> source
> function: op1 = mem\[op2\]

3. stw op1, op2
> R Type
> opcode: 2
> op1 -> destination, op2 -> source
> function: mem\[op1\] = op2

4. add op1, op2, op3
> R Type
> opcode: 3
> op1 -> destination, op2 -> operand, op3 -> operand
> function: op1 = op2 + op3

5. sll op1, op2, shift
> R Type
> opcode: 4
> op1 -> destination, op2 -> operand
> function: op1 = op2 << shift

6. neg op1, op2
> R Type
> opcode: 5
> op1 -> destination, op2 -> source
> function: op1 = -1 * op2

7. halt
> R Type
> opcode: 6
> function: ends the simulation

8. tst op1, op2
> R Type
> opcode 7
> op1 -> register to be tested, op2 -> flag location
> function: test op1 -> op2

9. cmp op1, op2, op3
> R Type
> opcode 8
> op1 -> first operand, op2 -> second operand, op3 -> flag location
> function compare op1, op2 -> op3

10. brc op1, op2, target
> I type
> opcode 9
> op1 -> flag location, op2 -> flag bit, target -> branch target
> function if (op1\[op2\]) pc = target
