#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main(int argc, char const *argv[]) {
	if (argc == 1)
		eerror("Usage: ./service <fifo-prefix>");
	char buf[128], tmp[128];
	strcpy(buf, argv[1]);
	strcat(buf, "r.dat");
	int fd_w = open(buf, O_WRONLY);
	if(fd_w == -1) eerror("WERROR !!");

	strcpy(tmp, argv[1]);
	strcat(tmp, "w.dat");
	int fd_r = open(tmp, O_RDONLY);
	if(fd_r == -1) eerror("RERROR !!");
	
	while(1){
		memset(buf, 128, '\0');
		read(fd_r, buf, 128);
		if(fork() > 0){
			wait();
		} else {
			dup2(fd_w, 1);
			execlp("wc", "wc", buf, NULL);
			eerror("execlp error");
		}
	}

	return 0;
}