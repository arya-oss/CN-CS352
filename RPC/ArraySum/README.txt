=====================================================
 Integer Array Summation using Remote Procedure Call

System: Linux 64bit, sun RPC library installed
Author: Rajmani Arya
=====================================================

problem description:
	client program call procedure (function) of server
	program which sums integer array and return summation.

COMPILE:
	make -f Makefile.add
	
RUN: (open each command in a new terminal)
	sudo ./add_server
	sudo ./add_client localhost
	
NOTE: 
	cmd line argument for client is hostname of server program
	If program gives some error then install RPCBIND
	sudo apt-get install rpcbind
