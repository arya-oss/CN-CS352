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
	3. Listing  files and dirs (reauires a argument) use ls . (for curr dir)
		cmd -> ls <dir_name>
	4. Changing Working dir
		cmd -> cd <dir_name>
	5. get file permission attr
		cmd -> getattr <file_or_dir>
	6. set file permission attr
		cmd -> setattr <file_or_dir> <permission_bits_octal>
	7. remove file or empty dirs
		cmd -> remove <file_or_empty_dirs>
	8. create new directory
		cmd -> mkdir <dir_name> <permission_bits_octal>
	9. create a new file
		cmd -> touch <file_name> <permission_bits_octal>

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
	
e.g command
	arya@nfs~$ ls .
	arya@nfs~$ ls /
	arya@nfs~$ cd /tmp
	arya@nfs~$ touch tmp.c 666
	arya@nfs~$ write tmp.c hello_arya 0 10
	arya@nfs~$ read tmp.c 0 10
	arya@nfs~$ mkdir tmpdir 666
	arya@nfs~$ getattr tmp.c
	arya@nfs~$ setattr tmp.c 666
	arya@nfs~$ remove tmp.c
	arya@nfs~$ remove tmpdir/ 
NOTE: 
	cmd line argument for client is hostname of server program
