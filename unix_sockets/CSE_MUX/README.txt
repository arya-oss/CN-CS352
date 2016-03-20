===========================================================
               	CSE Multiplex Mall
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> In a Mall there are 3 ticket counters and
customers comes and get into any of 3 screens C, S and E. Each
screen has some capacity. First client goes to C screen if it is
full then it passes customer to S screen, if it is full then it passes
customer to E Screen, if it is also full it signals ticket counter to stop
issue ticket.

For exit client don't do CTRC-C , write -1 instead. it will close sfd
and pthread safely.

Helper File:
utils.c utils.h
It different BSD and UNIX Domain Socket useful function for passing fds over unix domain socket.

Compile:
./build

RUN:( execute each command in a new terminal )
./tcserver
./cmx
./smx
./emx
./client 8001
./client 8002
./client 8003

NOTE:
If found any problem with codes, Do suggest !!
