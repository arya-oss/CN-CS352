#include "p1.h"
/**
 * Author: Rajmani Arya
 * filename: p1_2.c
 * Title: Write a C program that simulates
 * usage of System V IPC in inter process communication.
 * 1st program write value of x=0;
 * 2nd program read value of x and writes y=x+1
 * 1st program reads value of y and writes x=y+1
 * Note: files required for complete program
 * p1.h p1_1.c p1_2.c
 * run:
 * gcc p1_1.c -o p1
 * gcc p1_2.c -o p2
 * ./p1 &
 * ./p2
 */
int main(){
	create_semaphore(); 
	create_shared_memory();
	sem_init(); // intitialize both semaphore with 0
	
	int * x = attach_shared_memory1();
	int * y = attach_shared_memory2();
	/* Process 1 written value of x */
	*x = 0;
	sem_post(0); // release the semaphore_0 (initial_val=0)
	while(1) {
		sem_wait(1);
		printf("p1: Y = %d\n", *y);
		*x = *y+1;
		sem_post(0);
	}
	return 0;
}
