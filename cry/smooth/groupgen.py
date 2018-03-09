#!/usr/bin/python3
from sympy import factorint, isprime, randprime, root
from math import log
from custom import factor

#Anything below 23 bits should be fine
DLOG_LIMIT = 2**20
SIZE = 2048

#Using 128bit takes too long
N = 64

def gen_prime(a, b, smooth):
    while(True):
        p = randprime(a, b)
        
        factors = factorint(p - 1, limit=smooth)
        if(isprime(max(factors)) and max(factors) < smooth):
            return p

def gen_group(bits):
    if(bits % N != 0):
        raise Exception

    n_primes = bits//N
    
    a = root(2**(n_primes - 1), n_primes) * 2**(bits//n_primes - 1)
    b = 2**(bits//n_primes) - 1

    primes = []

    while(len(primes) < n_primes):
        primes.append(gen_prime(a, b, DLOG_LIMIT))
        print("FOUND ONE %d MORE TO GO" % (n_primes - len(primes)))
    

    modulus = 1
    for x in primes:
        modulus *= x

    return (modulus, primes)

print(gen_group(SIZE))
