#!/bin/sh
gcc client.c -o client
gcc server.c -o server

# remove previous created fifos

rm *.dat

# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c2; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c3; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c4; exec bash\""

exit
