===========================================================
                Alternate file printer Process
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create two process p1 , p2. p1 listen to
Unix Domain Socket and open a target file. p1 reads from stdin
no. of characters to print and it reads from file and print on
stdout and passes file descriptor over unix domain socket to process
p2. p2 reads no. of byte to print from stdin and reads from same file
and print that many character on stdout and send back file descriptor
to process p1 and this continues.

Helper File:
utils.c utils.h
It contains four useful function for passing fds over unix domain socket.

Compile:
./build

create a target file from process reads characters.
e.g;
$ man socket > target.txt

RUN:( execute each command in a new terminal )
./server /tmp/socket target.txt
./service /tmp/socket
...

After client execution it asks for service port that it wanted.

NOTE:
Some garbage character get printed from file. Leave it or Fix it.
