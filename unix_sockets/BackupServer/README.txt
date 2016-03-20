===========================================================
                BackUp Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a Server & backup Server serves.If Main Server 
is going to exceed it threshold, it passes currently all connected client
to backup server and signals backup server now backup server serves client.

For exiting clients don't do CTRC-C , write -1 instead. it will close sfd
and pthread safely.

Helper File:
utils.c utils.h
It contains different BSD and UNIX Domain Socket useful function for bind, connect and
accept.

Compile:
./build

server listening on port 8001

RUN:( execute each command in a new terminal )
./server
./backup
./client 8001
./client 8001
./client 8001

here threshold for server is 2. if 3rd client req comes it passes previous 2 clients
to backup server and serves 3rd client.

NOTE:
If found any problem with codes, Do suggest !!
