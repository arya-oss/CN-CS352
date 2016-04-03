#!/bin/sh

sudo rm /tmp/sockpath
gcc -c utils.c -o utils.o
gcc utils.o train.c -o train
gcc utils.o platform.c -o platform -lpthread
gcc utils.o station.c -o station
gcc cabletv.c -o cable
echo 'compiled !'
