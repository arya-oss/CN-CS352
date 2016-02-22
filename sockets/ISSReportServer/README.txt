===========================================================
                Internet Super Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a ISS which control service providing
servers. when a client make a request for a service, ISS creates
copy of that service and connects to client and Keep log output
of data sent to client.

Compile:
gcc server.c -o server
gcc client.c -o client
gcc service.c -o s1
cp s1 s2
cp s1 s3
cp s1 s4

RUN:( execute each command in a new terminal )
./server
./client 9501
./client 9502
./client 9501
./client 9502
...

NOTE:
if found any problem with it. Do debug by yourself and Make a PULL REQUEST

mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
