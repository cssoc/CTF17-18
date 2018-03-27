#!/bin/bash

gcc ROPe.c -o ROPe -fno-stack-protector -no-pie
docker build -t=rope .
