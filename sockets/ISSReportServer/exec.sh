#!/bin/sh
gcc server.c -o server
gcc client.c -o client
gcc service.c -o s1
gcc service1.c -o s2
gcc service2.c -o s3
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
