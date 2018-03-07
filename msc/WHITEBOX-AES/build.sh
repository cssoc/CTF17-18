gcc -o gen_keys gen_keys.c aes_locl.h
gcc -o encrypt encrypt.c aes_locl.h
strip encrypt
gcc -o solve solve.c aes_locl.h
./encrypt flag flag.enc
