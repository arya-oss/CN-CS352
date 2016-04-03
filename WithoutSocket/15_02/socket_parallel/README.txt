===========================================================
               		Server-Client TCP (Parallel connection)
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a tcp socket server which connects
client parallely i,e it doesn't wait if simultaneous request
arrives. For each client a new thread is being assigned on successful
connection. client sends a message to server and after recieving this,
 server replies back to client with message to uppercase letter.

Compile:
gcc server.c -o server
gcc client.c -o client

RUN:( execute each command in a new terminal )
./server
./client
./client
./client

NOTE:
if found any problem with it. Do debug by yourself and Make a PULL REQUEST

mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
