#!/bin/sh
gcc server.c -o server
gcc client.c -o client
gcc service.c -o s1 -lpthread
cp s1 s2
cp s1 s3
echo "compiled successfully !"
