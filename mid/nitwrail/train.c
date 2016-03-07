#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <unistd.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main(int argc, char const *argv[])
{
	if(argc < 4) 
		eerror("Usage: \n\t ./train [h|d|v] <bogey sequence> <fifo_name>\n");
	char buf[128];
	sprintf(buf, "%s_fifo", argv[1]);
	int ends = open(buf, O_WRONLY);
	
	sprintf(buf, "%s_fifo", argv[3]);
	mkfifo(buf, 0666);

	int rdwr = open(buf, O_RDWR);
	if(ends == -1 || rdwr == -1)
		eerror("open() error");

	write(ends, buf, strlen(buf));
	
	memset(buf, 0, 128);
	int n = read(rdwr, buf, 128);
	
	if(n == 0 | strcmp(buf, "exit") == 0) {
		eerror("Platform Busy! Try Again !");
	} else {
		printf("Platform %s available\n", buf);
		sleep(0.5);
		write(rdwr, argv[2], strlen(argv[2]));
		sleep(3);
	}
	int fd = open("a_fifo", O_WRONLY);
	sprintf(buf, "%s train left Platform !", argv[3]);
	write(fd, buf, strlen(buf));
	close(fd);
	printf("Leaving station !\n");
	close(ends);
	close(rdwr);
	return 0;
}
