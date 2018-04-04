from Crypto.PublicKey import RSA

with open('key.pem', 'r') as f:
    key = RSA.importKey(f.read())

flag = b"CSSOC{50m37h1n6_r34lly_b0r1n6}"

ct = key.encrypt(flag, 0)

with open('flag.enc', 'wb') as f:
    f.write(ct[0])


