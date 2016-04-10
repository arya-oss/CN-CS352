=====================================================
 Integer Array Summation using Remote Procedure Call

System: Linux 64bit, sun RPC library installed
Author: Rajmani Arya
=====================================================

Dependencies: rpcbind
sudo apt-get install rpcbind

problem description:
	client program call procedure (function) of server
	program which sums integer array and return summation.

Generate All Source file from add.x : (needed only when you want to write your own function and code)
	rpcgen -aC add.x
it will generate follwing files 
> add.h add_client.c add_server.c add_svc.c add_clnt.c add_xdr.c Makefile.add)

Or simply execute given program, follow these steps
COMPILE:
	make -f Makefile.add
	
RUN: (open each command in a new terminal)
	sudo ./add_server
	./add_client localhost
	
NOTE: 
	cmd line argument for client is hostname of server program
