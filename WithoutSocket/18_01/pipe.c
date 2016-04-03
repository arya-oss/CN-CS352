#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Author: Rajmani Arya
 * CSE 3rd Year, NITW
 * Title: Pipe i/o redirection through shell
 * Usage: ./pipe <cmd1> <cmd2> ...
 * E.g. you have 3 executable file on unix name p1, p2, p3
 * $ ./pipe ./p1 ./p2 ./p3
 * Note: This program doesn't check command line arguments p1, p2, p3
 * programs so take care of this by removing (int argc, char * argv[])
 * from other programs.
 * RUN:
 * gcc pipe.c -o pipe
 * others progs e.g p1.c p2.c etc.
 * gcc p1.c -o p1
 * gcc p2.c -o p2
 * ./pipe ./p1 ./p2
 */

char * tmp[] = { "0" };

int main(int argc, char * argv[]) {
	int i=0, in=0; int fd[2];
	if(argc == 1){
		printf("Usage: ./pipe <cmd> ...\n");
		exit(1);
	}
	for(i=1; i<argc-1; i++) {
		pipe(fd);
		if(fork() == 0) {
			dup2(in, 0);
			dup2(fd[1], 1);
			execvp(argv[i], tmp);
			printf("execvp error 1");
			exit(1);
		}
		close(fd[1]);
		in = fd[0];
	}
	if(in != 0)
		dup2(in, 0);
	execvp(argv[i], tmp);
	printf("execvp error 2\n"); // if everything is ok then it won't come
	return 0;
}
