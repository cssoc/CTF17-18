#!/bin/bash

openssl rsautl -encrypt -pubin -inkey key.pem -in flag.txt > flag.enc
