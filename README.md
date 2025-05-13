# ISAsim
---
ISAsim is a modular and highly configurable simulator/framwork for simulating various computer architectures and concepts. 
It is designed for rapid experimentation.

For the early stages, it is developed around a dummy architecture called testarch (see testarch.md).

## Building:
There isnt a build system yet. You can just build the c code by doing
```
gcc main.c -o sim
```
and assemble a test program by 
```
python assemble.py <infile> <outfile>
```
To build the disassembler, you can run
```
gcc disassemble.c -o dis
```

## Running:
To run an assembled program, you can use the following command:
```
./sim <filename>
```

## Disassembling a program:
The disassembler can output to stdout or a file. If a output file is provided, the output will be written there, if not, the output will be written to stdout.
```
./dis <infile> <outfile>
```

# Getting started:
Clone the repo with:
```
git clone https://github.com/Bob-The-Ai/ISAsim.git
```
Build the simulator (see Building)
Read the testarch specs and maybe the example test.s and write your own test program.
Assemble it and run it.

If you stumble on any bugs, please make an issue.
