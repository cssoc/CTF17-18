#!/usr/bin/python3
import json

from sympy.ntheory import discrete_log
from sympy.ntheory.modular import crt

from custom import n_order, factor

from Crypto.PublicKey import ElGamal

k_f = 'key.json'
m_f = 'msg.json'

key = json.load(open(k_f, 'r'))
msg = json.load(open(m_f, 'r'))

g = int(key['g'], 16)
p = int(key['p'], 16)
y = int(key['y'], 16)


print("FACTORISING P")
primes = factor(p)
print("DONE")

f_d = dict()
for x in set(primes):
    f_d[x] = primes.count(x)

print("CALCULATING ORDER")
order = n_order(g, p, f_d)
print("DONE")

dlogs = dict()

print("SOLVING DLOG")
#Uses pohlig-hellman internally
x = discrete_log(p, y, g, order=order)

print("DONE WITH DLOG")
print("SECRET KEY %x" %x)

key_obj = ElGamal.construct((p, g, y, x))
print(key_obj.decrypt((bytes.fromhex(msg['c1']), bytes.fromhex(msg['c2']))))
