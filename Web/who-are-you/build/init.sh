#!/bin/sh

echo $GZCTF_FLAG > /app/flag.txt
unset GZCTF_FLAG

python /app/app.py