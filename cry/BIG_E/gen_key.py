#!/usr/bin/python3

from Crypto.PublicKey import RSA
from math import log
from sympy import root, lcm, N
from random import getrandbits

SIZE = 2048

def egcd(b, a):
    x0, x1, y0, y1 = 1, 0, 0, 1
    while a != 0:
        q, b, a = b // a, a, b % a
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    return  b, x0, y0

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

key = RSA.generate(SIZE)
D_LIMIT = root(key.n, 4) // 3

PHI = lcm(key.p - 1, key.q - 1)

found = False

while(not found):
    d = getrandbits(int(N(D_LIMIT, maxn=500)).bit_length() - 1)
    try:
        e = modinv(d, PHI)
        found = True
    except:
        pass

n_key = RSA.construct((key.n, int(e), d))

with open('key.pem', 'wb') as f:
    f.write(n_key.publickey().exportKey())

with open('p_key.pem', 'wb') as f:
    f.write(n_key.exportKey())
