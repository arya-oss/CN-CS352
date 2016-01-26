#!/bin/sh
gcc server.c -o server
gcc fclient.c -o fclient
gcc pclient.c -o pclient
gcc oclient.c -o oclient
rm *.dat
echo "compiled successfully !"
