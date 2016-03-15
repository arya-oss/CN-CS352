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

---------------
How I did
---------------

creates a server which listen to some port and keeps a global variable which keep
tracks that it is busy or not, if it busy then it stores current client port. if
any client request comes then if server is available it simply accepts and starts
servicing otherwise busy, then it write current client port to new client and closes the
connection for new client. Now new client takes permission from old connected client, now
two scenarios takes place.
	1. old client client grants permission then old client close all sockets and exits and
	new client makes new request to server and also serves on same port on which it connected
	to server (kernel allocated).
	2. old client denies permission then new client exits. old one takes service as it was.

Compile:
./build

RUN:( execute each command in a new terminal )
./server
./client 9500 1
./client 9500 1
...
<client format> --> ./client <server_port> <permission for any client 0->no, 1->yes>

NOTE:
	Permission is predetermined in form of 2nd cmd line argument either yes or no.
	For exit client at any time, write -1 as service i/p.
