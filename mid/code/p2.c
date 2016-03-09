/*
 * Helper Process of P2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFSIZE 1024

/**
 * Author: Rajmani Arya
 * Date: 8th March 2016
 */

int main(int argc, char * argv[]) {
	int fd = open("fifo", O_WRONLY);
	while(1) {
		write(fd, "p2_hello", 8);
		sleep(2);
	}
	return 0;
}
