#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main (){
	int fd = open("c5.dat", O_RDONLY);
	char buf[1024];
	while(1) {
		if(read(fd, buf, 1024) > 0) {
			printf("%s", buf);
		}
		sleep(0.5);
	}
    return 0;
}
