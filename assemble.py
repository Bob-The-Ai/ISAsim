import sys
import re

# Register mapping: s0-sf (sp), t0-tf (ra)
registers = {f"s{i:x}": i for i in range(16)}
registers.update({f"t{i:x}": 16 + i for i in range(16)})
registers["sp"] = registers["s0"]
registers["ra"] = registers["t0"]

# Opcodes
opcodes = {
    "ldi": 0,
    "ldw": 1,
    "stw": 2,
    "add": 3,
    "sll": 4,
    "neg": 5,
    "hlt": 6,
}

# Helper to encode fields
def encode_r(op, op1, op2, op3=0, shift=0, flags=0):
    return (
        (op << 26)
        | (op1 << 21)
        | (op2 << 16)
        | (op3 << 11)
        | (shift << 6)
        | flags
    )

def encode_i(op, op1, op2, imm):
    imm &= 0xFFFF  # 16-bit immediate
    return (op << 26) | (op1 << 21) | (op2 << 16) | imm

# Assembler
def assemble_line(line):
    line = line.strip()
    if not line or line.startswith("#"):
        return None
    tokens = re.split(r"[,\s]+", line)
    instr = tokens[0].lower()
    args = tokens[1:]

    opcode = opcodes.get(instr)
    if opcode is None:
        raise ValueError(f"Unknown instruction: {instr}")

    def reg(r): return registers[r.lower()]

    if instr == "ldi":
        rd, imm = reg(args[0]), int(args[1], 0)
        return encode_i(opcode, rd, 0, imm)

    elif instr in ["ldw", "stw", "neg"]:
        rd, rs = reg(args[0]), reg(args[1])
        return encode_r(opcode, rd, rs)

    elif instr == "add":
        rd, rs1, rs2 = reg(args[0]), reg(args[1]), reg(args[2])
        return encode_r(opcode, rd, rs1, rs2)

    elif instr == "sll":
        rd, rs, shamt = reg(args[0]), reg(args[1]), int(args[2], 0)
        return encode_r(opcode, rd, rs, shift=shamt)

    elif instr == "hlt":
        print("fucking hlt")
        return encode_r(opcode, 0, 0, 0, 0, 0)

    else:
        raise ValueError(f"Unsupported instruction: {instr}")

# Entry point
def main():
    input_file = sys.argv[1]
    output_file = sys.argv[2]

    with open(input_file) as f:
        lines = f.readlines()

    machine_code = []
    for i, line in enumerate(lines):
        try:
            word = assemble_line(line)
            if word is not None:
                machine_code.append(word)
        except Exception as e:
            print(f"Error on line {i+1}: {line.strip()}\n{e}")
            sys.exit(1)

    with open(output_file, "wb") as f:
        for word in machine_code:
            f.write(word.to_bytes(4, byteorder='little'))
            #f.write(word.to_bytes(4, byteorder='big'))

    print(f"Assembled {len(machine_code)} instructions to {output_file}")

if __name__ == "__main__":
    main()
