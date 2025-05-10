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

## Running:
To run an assembled program, you can use the following command:
```
./sim <filename>
```
