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
	int i; char buf[BUFSIZE];
	for(i=0; i<3; i++){
		read(0, buf, BUFSIZE);
		write(1, buf, BUFSIZE);
	}
	return 0;
}
