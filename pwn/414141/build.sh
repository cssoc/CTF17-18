#!/bin/bash

gcc 414141.c -o 414141 -no-pie -fno-stack-protector
docker build -t=414141 .
