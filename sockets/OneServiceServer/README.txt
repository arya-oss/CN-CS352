===========================================================
                One Service Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a server which provide service for
client only one client at a time. if server is giving service to
one client and another client request comes then 2nd client has to
take permission from first client for service, if first client
grants permission then server start servicing 2nd client.

Compile:
./build

RUN:( execute each command in a new terminal )
./server
./client 9500 1
...
<client format> --> ./client <server_port> <permission for any client 0->no, 1->yes>

NOTE:
	NOT WORKING NOW, CURRENTLY DEBUGGING IS GOING ON.
