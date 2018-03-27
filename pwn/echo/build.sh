#!/bin/bash

gcc echo.c -o echo -no-pie
docker build -t=echo .
docker run --rm --entrypoint cat echo /lib/x86_64-linux-gnu/libc.so.6 > libc.so.6
if md5sum -c md5_check;
then
	:
else
	echo "Please contact me about the libc change"
fi
