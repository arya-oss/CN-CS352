#include "utils.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		printf("Usage: ./client <port>\n");
		exit(1);
	}
	int port = atoi(argv[1]);
	int sfd = tcpsocket_connect(port);
	if(sfd < 0) {
		printf("tcpclient_connect()\n");
		exit(2);
	}
	char buf[BUFSIZE];
	sleep(0.5);
	printf("client started\n");
	while(1) {
		scanf("%s", buf);
		if(atoi(buf) == -1) {
			write(sfd, "-1", 2);
			close(sfd);
			printf("client exiting...\n");
			exit(1);
		}
		write(sfd, buf, strlen(buf));
		read(sfd, buf, BUFSIZE);
		printf("%s\n", buf);
		memset(buf, 0, BUFSIZE);
	}
	return 0;
}