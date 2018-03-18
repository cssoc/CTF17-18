#!/usr/bin/python2

FLAG = b"CSSOC{Who_needs_DEP_anyways?}"
FLAG_LEN = len(FLAG)

FLAG_XORS = {
3: 11,
18: 122,
15: 117,
28: 18,
23: 37,
21: 34,
27: 89,
10: 90,
0: 71,
9: 90,
19: 10,
7: 240,
14: 76,
26: 76,
2: 21,
12: 175,
24: 76,
1: 215,
20: 234,
4: 3,
5: 217,
13: 73,
16: 217,
22: 195,
6: 182,
17: 189,
8: 67,
25: 134,
11: 11}

order = [3, 18, 15, 28, 23, 21, 27, 10, 0, 9, 19, 7, 14, 26, 2, 12, 24, 1, 20, 4, 5, 13, 16, 22, 6, 17, 8, 25, 11]


FUNC_XORS = [ord('C'), FLAG_LEN ^ 0xff]

for x in order:
    FUNC_XORS.append(FLAG_XORS[x] ^ ord(FLAG[x]))

from pwn import *

e = ELF("./write_exec")

print(FUNC_XORS)

FUNS = ['check_len']
for x in order:
    FUNS.append('check_char' + str(x))

FUNS.append('solved')

for y, x in enumerate(FUNS):
    fun = e.functions[x]
    data = e.read(fun.address, fun.size)
    ct = ""
    print("XORING %s with %d" % (x, FUNC_XORS[y]))
    for z in data:
        ct += chr( (ord(z) ^ FUNC_XORS[y]) & 255)
    e.write(fun.address, ct)

e.save("w&x")
