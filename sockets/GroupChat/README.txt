===========================================================
                Groups Chat Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a group chat server. Server maintains
some chat group. client request group servers after getting response
from server it asks group id (port no.) for group and start chatting
inside particular group. 

Compile:
gcc server.c -o server
gcc client.c -o client -lpthread
gcc service.c -o s1
cp s1 s2
cp s1 s3

RUN:( execute each command in a new terminal )
./server
./client 9500
./client 9500
...
After getting response from server enter group id i.e portno (9501, 9502, 9503);

NOTE:
if found any problem with it. Do debug by yourself and Make a PULL REQUEST

mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
