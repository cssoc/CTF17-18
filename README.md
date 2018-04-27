# CTF17-18
## CSSOC CTF from the 27th of April 2018

Some challenges were tested with scripts

### Web

#### Ajax

No authentication. Just access https://ajax.idocker.hacking-lab.com/api/get?id=1

#### Blind

Blind-SQL-injection in username.

#### Cross

XSS via message form. Steal admins session cookie.

#### inject_me

SQL injection.

#### Proxy

SSRF, bypass filter with 0. Use gopher so that CR/LF is urldecoded.

#### serial

RCE via cookie-deserialisation.

### Crypto

#### BIG_E

Use wieners attack due to low private exponent.

#### PERIOD

Bruteforce keyspace, LSFR has small state.

#### School assignment

ROT13

#### smooth

Factorise modulus (which is smooth) with pollards p-1, then use pohlig-hellman to recover message.

### Pwn

#### 414141

Bufferoverflow, overwrite return address.

#### echo

printf vuln. Overwrite GOT entry for exit with an address from libc.

#### guess

printf vuln, leak string from stack.

#### ROPe

build simple ROP chain to call system(sh).

#### syscall

Use bufferoverflow to overwrite return address, use syscall gadget to call sigreturn to set processor state to call execve(/bin/sh)

#### TELLME

Just send shellcode.

### Misc

#### cmp

ltrace

#### not-quite-steg

gzip archive at end of image.

#### WHITEBOX-AES

obtain offset for roundkeys by reverseengineering the binary then dump key

#### W&X

Runtime decryption, use a debugger.
