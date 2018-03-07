#!/usr/bin/python2

from pwn import *

e = ELF('./encrypt')

NUM_KEYS = 44

#found by checking assembly
BASE = 0x00002560

for x in range(NUM_KEYS):
    print(hex(e.u32(BASE + 4*x)))
