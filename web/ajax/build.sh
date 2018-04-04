#!/bin/bash
rm db.sqlite
sqlite3 db.sqlite < init.sql
docker build -t=ajax .
