#!/bin/sh
gcc server.c -o server
gcc fclient.c -o fclient
gcc pclient.c -o pclient
gcc oclient.c -o oclient
rm *.dat
echo "compiled successfully !"

# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./fclient c1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./oclient; exec bash\""

exit
