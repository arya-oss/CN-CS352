#!/bin/sh

gcc server.c -o server
gcc client.c -o client -lpthread

gcc s1.c -o s1
gcc s2.c -o s2
gcc s3.c -o s3
# remove previous created fifos
rm *.dat
# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 2
gnome-terminal -e "bash -c \" ./client s1 c1; exec bash\""
sleep 2
gnome-terminal -e "bash -c \" ./client s2 c2; exec bash\""
sleep 2
gnome-terminal -e "bash -c \" ./client s3 c3; exec bash\""

exit
