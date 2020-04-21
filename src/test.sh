#!/bin/sh

gcc -o bfc bfc.c
./bfc hello.bf > asm.s
gcc -o bin asm.s
./bin