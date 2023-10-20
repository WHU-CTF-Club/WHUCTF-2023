#!/bin/sh

echo $GZCTF_FLAG > /home/ctf/flag
chown -R ctf:ctf /home/ctf/flag
export GZCTF_FLAG=""

cd /home/ctf
while [ 1 ]
do
    su ctf -c "./server" 2>&1 /dev/null
done