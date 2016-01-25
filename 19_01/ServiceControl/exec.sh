#!/bin/sh
gcc server.c -o server
gcc client.c -o client
gcc service.c -o service
rm *.dat
ipcrm -M 0x124
ipcrm -M 0x123
echo "compiled successfully !"
