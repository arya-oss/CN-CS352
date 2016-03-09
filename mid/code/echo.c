/*
 * Helper Echo Service
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

/**
 * Author: Rajmani Arya
 * Date: 8th March 2016
 */

int main(int argc, char * argv[]) {
	int i;
	for(i=0; i<5; i++){
		write(0, "echo_hello", 10);
		sleep(1);
	}
	return 0;
}
