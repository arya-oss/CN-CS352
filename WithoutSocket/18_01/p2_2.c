#include "p2.h"
/**
 * Author: Rajmani Arya
 * filename: p2_2.c
 * Title: Write a C program that simulates
 * usage of System V IPC in inter process communication.
 * 1st program write value of x=0;
 * 2nd program read value of x and writes y=x+1
 * 1st program reads value of y and writes x=y+1
 * NOTE: Don't Use semaphores do with signals only.
 * files required for complete program
 * p2.h p2_1.c p2_2.c
 * run:
 * gcc p2_1.c -o p1
 * gcc p2_2.c -o p2
 * ./p1 &
 * ./p2
 */
int main(){
	create_shared_memory();
	
	int * x = attach_shared_memory1();
	int * y = attach_shared_memory2();
	/* Process 1 written value of x */
	int p1_pid = *x;
	*y = getpid();
	kill(p1_pid, SIGCONT);

	while(1) {
		raise(SIGSTOP);
		printf("p1: X = %d\n", *x);
		*y = *x+1;
		kill(p1_pid, SIGCONT);
	}
	return 0;
}
