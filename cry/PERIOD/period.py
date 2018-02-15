#!/usr/bin/python3

from flag import flag
from lsfr import LSFR_5bit

from Crypto import Random
from Crypto.Random import random
from Crypto.Cipher import AES

import sys

gen = LSFR_5bit(random.getrandbits(5))
while(gen.state == 0):
    gen = LSFR_5bit(random.getrandbits(5))


key = b""
while len(key) < 32:
    key += (gen.get_byte()).to_bytes(1, byteorder='big')

IV = Random.new().read(AES.block_size)
cipher = AES.new(key, AES.MODE_CBC, IV)

while(len(flag) % 16 != 0):
    flag += "\00"

sys.stdout.buffer.write(IV + cipher.encrypt(flag))
