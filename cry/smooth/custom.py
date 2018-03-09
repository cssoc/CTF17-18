#Custom version of some functions since sympys implementations have some flaws
from sympy.core.compatibility import as_int
from sympy import sieve, igcd, isprime, factorint

import math

#Returns true or false depending on whether B was too big or too small
def pollard_pm1(n, B=10, a=2, retries=0, seed=1234):
    n = int(n)
    if n < 4 or B < 3:
        raise ValueError('pollard_pm1 should receive n > 3 and B > 2')
    for i in range(retries + 1):
        aM = a
        for p in sieve.primerange(2, B + 1):
            e = int(math.log(B, p))
            aM = pow(aM, pow(p, e), n)
        g = igcd(aM - 1, n)
        if g == n:
            return True
        elif g == 1:
            return False
        elif 1 < g < n:
            return int(g)

#Uses custom p-1 and adjusts B automatically
def factor(n):
    facts = []
    
    while(not isprime(n)):
        b = 0
        B = 10
        cand = pollard_pm1(n, B=B)
        while(cand == True or cand == False):
            if(cand):
                if(b > B):
                    b = B
                    B = B//2
                else:
                    t = (b + B)//2
                    b = B
                    B = t
            else:
                if(b < B):
                    b = B
                    B = B*2
                else:
                    t = (b + B)//2
                    b = B
                    B = t
            
            cand = pollard_pm1(n, B=B)
            

        if(isprime(cand)):
            print("FOUND FACTOR")
            facts.append(cand)
        else:
            cand_f = factor(cand)
            for x in cand_f:
                facts.append(x)

        n = n//cand
    
    facts.append(n)

    return facts

#accepts factored modulus as an argument
def n_order(a, n, f):
    from collections import defaultdict
    a, n = as_int(a), as_int(n)
    if igcd(a, n) != 1:
        raise ValueError("The two numbers should be relatively prime")
    factors = defaultdict(int)
    for px, kx in f.items():
        if kx > 1:
            factors[px] += kx - 1
        fpx = factorint(px - 1)
        for py, ky in fpx.items():
            factors[py] += ky
    group_order = 1
    for px, kx in factors.items():
        group_order *= px**kx
    order = 1
    if a > n:
        a = a % n
    for p, e in factors.items():
        exponent = group_order
        for f in range(e + 1):
            if pow(a, exponent, n) != 1:
                order *= p ** (e - f + 1)
                break
            exponent = exponent // p
    return order
