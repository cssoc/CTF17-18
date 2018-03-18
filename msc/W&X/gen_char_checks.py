#!/usr/bin/python3

import random

FLAG_LEN = 29

def gen(x):
    ran = random.randint(0, 255)
    st = """
char check_char""" + str(x) + \
    """(char *input)
{
    return input[""" + str(x) + "] ^ " + str(ran) + ";\n}"

    return st

funcs = []

for x in range(FLAG_LEN):
    funcs.append(gen(x))

random.shuffle(funcs)

for x in funcs:
    print(x)
