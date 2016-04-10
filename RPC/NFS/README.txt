=====================================================
 Network File System using Remote Procedure Call

System: Linux 64bit, sun RPC library installed
Author: Rajmani Arya
=====================================================

Dependencies: rpcbind
sudo apt-get install rpcbind

problem description:
	Implement Network file system using sun RPC.
	
Implemented Feature:
	1. Read a file
		cmd -> read <filename> <start_offset> <no_of_bytes>
	2. Write something into a file
		cmd -> write <filename> <byte_to_write_without_blanks> <start_offset> <no_of_bytes>
	3. Listing  files and dirs
		cmd -> ls <dir_name>
		
Features to be implement:-
	1. mkdir
	2. rmdir
	3. rm
	4. cd
	5. stat 

Generate All Source file from nfs.x : (needed only when you want to write your own function and code)
	rpcgen -aC nfs.x
it will generate follwing files 
> nfs.h nfs_client.c nfs_server.c nfs_svc.c nfs_clnt.c nfs_xdr.c Makefile.nfs)

Or simply execute given program, follow these steps
COMPILE:
	make -f Makefile.nfs
	
RUN: (open each command in a new terminal)
	sudo ./nfs_server
	./nfs_client localhost
	
NOTE: 
	cmd line argument for client is hostname of server program
