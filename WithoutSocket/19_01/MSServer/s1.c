#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main(int argc, char const *argv[]) {
	if (argc == 1)
		eerror("Usage: ./service <fifo-prefix>");
	char buf[1024], tmp[128];
	
	sprintf(buf, "%sr.dat", argv[1]);
	int fd_w = open(buf, O_WRONLY);
	if(fd_w == -1) eerror("WERROR !!");
	
	sprintf(tmp, "%sw.dat", argv[1]);
	int fd_r = open(tmp, O_RDONLY);
	if(fd_r == -1) eerror("RERROR !!");
	printf("You are good to go.\n");
	while(1){
		memset(buf, 128, '\0');
		if(read(fd_r, buf, 128) == 0){
			sleep(0.5);
			continue;
		}
		if(strcmp(buf, "exit") == 0) {
			printf("Service 1 Exiting . . .\n");
			exit(127);
		}
			
		if(fork() > 0) {
			wait();
		} else {
			memset(buf, 1024, '\0');
			write(fd_w, buf, 1024);
			dup2(fd_w, 1);
			execlp("ls", "ls", buf, NULL);
			eerror("execlp error");
		}
	}

	return 0;
}
