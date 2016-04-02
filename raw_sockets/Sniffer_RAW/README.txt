====================================================
Packet Sniffer and Socket RAW without IP_HDR_INCL

Author: Rajmani Arya
Date: 2nd Apr 2016
====================================================

NOTE: Run these programs with root permissions (sudo).
Sniffer catches both incoming and outgoing packets for
incoming packet only repalce ETH_P_ALL to ETH_P_IP.

Problem Statment:
	Create your own custom network protocol using raw sockets
without IP Header Include and write a client-server program 
which agrees on same protocol number (in this case 146). 
client sends message to server and server displays IP Header and 
msg in a nice format. Packet sniffer can catch any type of protocol,
e.g your current client-server program, your web browser packet, mails etc.

Compile:
./build

Run: (each command in a new terminal )
sudo ./server 8001
sudo ./sniff
sudo ./client 8001

NOTE:
	Here server binded on port 8001. Its not needed when client wants
	to send message for anyone on protocol 146.
