
MIPS Processor Simluated in C
RPI Computer Organization Project 2021
Group:

Julian Matthews
Nadia Choudhury
Priya Goel
Cherry Bommu
Keegan Herron

This project implements a full gate-level circuit representing the datapath for a reduced
MIPS ISA. Our team expanded on the provided datapath by developing our own implementation
for jump register and jump and link commands.


Usage: 

    ./project < input.txt

Inputs are a series of MIPS commands

Output is the state of the program counter, the current instruction in binary, and the data and temporary registers.

Original was one file project.c, which is compiled as project. This has been seperated into multiple files for readability. 