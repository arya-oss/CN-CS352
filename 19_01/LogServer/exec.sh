#!/bin/sh
#!/bin/sh

gcc server.c -o server
gcc client.c -o client

gcc s1.c -o s1
gcc s2.c -o s2
gcc s3.c -o s3
# remove previous created fifos
rm *.dat
# open each server and client in new terminal

gnome-terminal -e "bash -c \" ./server; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c1 s1; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c2 s3; exec bash\""
sleep 1
gnome-terminal -e "bash -c \" ./client c3 s3; exec bash\""

exit
