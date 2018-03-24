#!/usr/bin/python3

from Crypto.PublicKey import RSA
from sympy import Symbol, Rational
from sympy.solvers import solve
from sympy.ntheory.continued_fraction import continued_fraction_iterator, continued_fraction_convergents

with open('key.pem', 'rb') as f:
    data = f.read()

key = RSA.importKey(data)

cf = continued_fraction_iterator(Rational(key.e, key.n))
conv = continued_fraction_convergents(cf)

for x in conv:
    if(x == 0):
        continue

    k = x.p
    d = x.q

    phi = (key.e * d - 1) // k
    
    y = Symbol('y', integer=True)
    roots = solve(y**2 - ((key.n - phi) + 1) * y + key.n, y)
    if(roots and key.n == roots[0] * roots[1]):
        print("FACTORED")
        break

d = Rational(key.e).invert((roots[0] - 1) * (roots[1] - 1))
p_key = RSA.construct((key.n, key.e, d))

with open('flag.enc', 'rb') as f:
    data = f.read()

print(p_key.decrypt(data))
