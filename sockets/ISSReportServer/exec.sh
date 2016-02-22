#!/bin/sh
gcc server.c -o server
gcc client.c -o client
gcc service.c -o s1
cp s1 s2
cp s1 s3
cp s1 s4
echo "compiled successfully !"

# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client 9501; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client 9502; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client 9503; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client 9504; exec bash\""

exit
