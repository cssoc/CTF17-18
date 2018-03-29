#!/usr/bin/python2

from pwn import *
from Crypto.Cipher import AES

e = ELF('./encrypt')

#found by checking assembly
BASE = 0x00002560

key = ""
for x in range(4):
    key += e.read(BASE + x*4, 4)[::-1]

log.info("KEY = %s" % (key.encode('hex')))

with open('flag.enc', 'rb') as f:
    data = f.read()

cipher = AES.new(key, AES.MODE_ECB)
print cipher.decrypt(data)
