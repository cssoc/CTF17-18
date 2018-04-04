#!/bin/bash

#nasm -f elf64 syscall.s
#ld syscall.o -o syscall
#rm syscall.o
#strip syscall
docker build -t=syscall .
