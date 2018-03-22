#!/bin/bash

pass="/51LLebGwLAHovrwrSdJwW0uJ4eRYYUw9WookRY9AOU="
flag="CSSOC{MAKE_SSRF_GREAT_AGAIN}"

while [ true ]; do
printf "set password 0 0 ${#pass}\r\n\
$pass\r\n\
set flag 0 0 ${#flag}\r\n\
$flag\r\n" | nc -q1 localhost 11211
sleep 30
done
