#!/bin/sh
gcc server.c -o server
gcc client.c -o client
gcc s1.c -o s1
gcc s2.c -o s2
gcc s3.c -o s3
rm *.dat
echo "compiled successfully !"