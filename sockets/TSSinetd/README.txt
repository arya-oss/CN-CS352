===========================================================
                Threaded Super Server 
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a TSS which control service providing
servers. when a client make a request for a service, ISS creates
a thread and connects to client.In this case limited no. of services
instance are there so after max client connection, service can not 
handle further request.

Compile:
gcc server.c -o server
gcc client.c -o client
gcc service.c -o s1
cp s1 s2
cp s1 s3

RUN:( execute each command in a new terminal )
./server
./client 9500
./client 9500
./client 9500
./client 9500
...

After client execution it asks for service port that it wanted.

NOTE:
if found any problem with it. Do debug by yourself and Make a PULL REQUEST

mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
