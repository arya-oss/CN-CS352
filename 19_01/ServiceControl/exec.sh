#!/bin/sh
gcc server.c -o server
gcc client.c -o client -lpthread
gcc service.c -o service

# remove previous fifos
rm *.dat

# remove shared memory
ipcrm -M 0x124
ipcrm -M 0x123

echo "compiled successfully !"

# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./service 1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./service 2; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c2; exec bash\""

exit
