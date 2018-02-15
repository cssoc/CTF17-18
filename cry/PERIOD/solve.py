#!/usr/bin/python3
from Crypto.Cipher import AES
from lsfr import LSFR_5bit
import socket

ip = "127.0.0.1"
port = 1337

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, port))
data = s.recv(1024)
s.close()

iv = data[:16]
ct = data[16:]

def get_key(state):
    key = b""
    gen = LSFR_5bit(state)
    while len(key) < 32:
        key += (gen.get_byte()).to_bytes(1, byteorder='big')
    return key

for x in range(1, 2**5):
    key = get_key(x)
    cipher = AES.new(key, AES.MODE_CBC, iv)
    pt = cipher.decrypt(ct)
    if(b"CSSOC{" in pt):
        print(pt)
        break
