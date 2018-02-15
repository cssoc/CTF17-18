#!/bin/bash

docker build -t=period .
tar zcf period.tgz lsfr.py period.py
