===========================================================
                Threaded Super Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> Server serves three services to clients on
different ports. client request for any service with service port
server accepts client request and creates service instance if it is
not created before and then send client socket file descriptor to
particular service and service gets connected to client and serves.

For exit client don't do CTRC-C , write -1 instead. it will close sfd
and pthread safely.

Three Different Services:
service1(listen to 8001 port) -> converts to UPPERCASE letters
service2(listen to 8002 port) -> converts to LOWERCASE letters
service3(listen to 8003 port) -> simply reply same letters

Helper File:
utils.c utils.h
It different BSD and UNIX Domain Socket useful function for passing fds over unix domain socket.

Compile:
./build

RUN:( execute each command in a new terminal )
./server
./client 8001
./client 8002
./client 8003

NOTE:
If found any problem with codes, Do suggest !!
