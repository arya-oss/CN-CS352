#include "nfs.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int getCommandType(char * str) {
	if(strcmp("ls", str) == 0) {
		return 1; // ls -> 1
	} else if(strcmp("read", str) == 0) {
		return 2; // read -> 2
	} else if(strcmp("write", str) == 0) {
		return 3; // write -> 3
	} else if(strcmp("exit", str) == 0) {
		return 0; // exit -> 0
	} else if(strcmp("cd", str) == 0) {
		return 4;
	}
	return -1;
}

void
nfs_arya_1(char *host)
{
	CLIENT *clnt; char * args[4];
	bufsize *result_1;
	file_args_r nfs_ls_1_arg;
	int  *result_2,i;
	file_args_w  nfs_write_1_arg;
	bufsize  *result_3;
	file_args_r  nfs_read_1_arg;
	int  *result_4;
	file_args_r  nfs_cd_1_arg;
	char buffer[1024]; char * tok;
#ifndef	DEBUG
	clnt = clnt_create (host, nfs_arya, nfs, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	while(1) {
		printf("arya@nfs~$ ");
		fgets(buffer, 1024, stdin);
		tok = strtok(buffer, " \n");
		char * savecmd = tok;
		for (i = 0; i < 4; ++i){
			tok = strtok(NULL, " ");
			if(tok == NULL)	break;
			args[i] = tok;
		}
		switch(getCommandType(savecmd)) {
			case 0: // exit
				printf("Byee !\n");
				exit(1);
				break;
			case 1: // listing files
				if(i!=1){
					printf("usage: ls <dir_name>\n");
					break;
				}
				nfs_ls_1_arg.offset = nfs_ls_1_arg.size = 0;
				strtok(args[0], "\n");
				strcpy(nfs_ls_1_arg.filename, args[0]);

				result_1 = nfs_ls_1(&nfs_ls_1_arg, clnt);
				if (result_1 == (bufsize *) NULL) {
					clnt_perror (clnt, "call failed");
				} else {
					if(result_1->size <= 0) {
						printf("Error Occured in reading directory !!\n");
					} else {
						printf("Read %d files and dirs\n", result_1->size);
						printf("%s\n", result_1->buffer);
					}
				}
				break;
			case 2:
				if(i!=3) {
					printf("usage: read <filename> <offset> <size>\n");
					break;
				}
				strtok(args[0], "\n");
				strcpy(nfs_read_1_arg.filename, args[0]);
				nfs_read_1_arg.offset = atoi(args[1]);
				nfs_read_1_arg.size = atoi(args[2]);
				// printf("%s %d %d\n", nfs_read_1_arg.filename, nfs_read_1_arg.offset, nfs_read_1_arg.size);
				result_3 = nfs_read_1(&nfs_read_1_arg, clnt);
				if (result_3 == (bufsize *) NULL) {
					clnt_perror (clnt, "call failed");
				} else {
					if(result_3->size < 0) {
						printf("File Reading error Occured !\n");
					} else {
						printf("Read %d bytes\n", result_3->size);
						printf("%s\n", result_3->buffer);
					}
				}
				break;
			case 3:
				if(i!=4){
					printf("usage: write <filename> <msg_without_blanks> <offset> <size>\n");
					break;
				}
				strtok(args[0], "\n");
				strcpy(nfs_write_1_arg.filename, args[0]);
				strtok(args[1], "\n");
				strcpy(nfs_write_1_arg.buffer, args[1]);
				nfs_write_1_arg.offset = atoi(args[2]);
				nfs_write_1_arg.size = atoi(args[3]);
				// printf("%s %s %d %d\n", nfs_write_1_arg.filename, nfs_write_1_arg.buffer, nfs_write_1_arg.offset, nfs_write_1_arg.size);
				result_2 = nfs_write_1(&nfs_write_1_arg, clnt);
				if (result_2 == (int *) NULL) {
					clnt_perror (clnt, "call failed");
				} else {
					if(*result_2 < 0) {
						printf("Wrting Error occured, File not exist or access denied.\n");
					} else {
						printf("%d byte written !\n", *result_2);
					}
				}
				break;
			case 4: // cd
				if(i!=1) {
					printf("usage: cd <dir>\n");
					break;
				}
				strtok(args[0], "\n");
				strcpy(nfs_cd_1_arg.filename, args[0]);
				result_4 = nfs_cd_1(&nfs_cd_1_arg, clnt);
				if (result_4 == (int *) NULL) {
					clnt_perror (clnt, "call failed");
				} else {
					if(*result_4 < 0) {
						printf("Wrong path !\n");
					} else {
						printf("Curr directory changed\n");
					}
				}
				break;
			default:
				printf("Invalid Command\n");
		}
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	nfs_arya_1 (host);
exit (0);
}