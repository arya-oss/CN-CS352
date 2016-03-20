#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <pthread.h>
#include <error.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "utils.h"

#define BUFSIZE 1024
#define MAXS 3
int ports[] = {8001, 8002, 8003};
volatile int start[3] = {0};
volatile int nusfd[3] = {0};

int main(int argc, char const *argv[])
{
	char buf[BUFSIZE]; int i, res, n, nsfd, sfd[MAXS], usfd[MAXS];
	struct sockaddr_in addr; int cli_len;
	for (i = 0; i < MAXS; ++i) {
		sprintf(buf, "/tmp/sockpath%d", i);
		usfd[i] = init_sockbind(buf);
		if(usfd[i] < 0) {
			printf("init_sockbind()\n");
			exit(1);
		}
		sfd[i] = tcpsocket_bind(ports[i]);
		if(sfd[i] < 0) {
			printf("tcpsocket_bind()\n");
			exit(1);
		}
	}
	int max_sfd = -1;
	for (i = 0; i < MAXS; ++i){
		if(max_sfd < sfd[i])
			max_sfd = sfd[i]; 
	}
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		for (i = 0; i < MAXS; ++i) {
			FD_SET(sfd[i], &rfds);
		}
		res = select(max_sfd+1, &rfds, NULL, NULL, NULL);
		if(res < 0) {
			sleep(0.25);
			continue;
		}
		for (i = 0; i < MAXS; ++i) {
			if(FD_ISSET(sfd[i], &rfds)) {
				if(start[i] == 0) { // service not started yet
					if(fork() == 0) {
						sprintf(buf, "/tmp/sockpath%d", i);
						char cmd[32];
						sprintf(cmd, "service%d", i+1);
						execl(cmd, cmd, buf, NULL);
						perror("execl() ");
						exit(1);
					} else {
						printf("service %d started\n", ports[i]);
						start[i] = 1;
						sleep(1);
					}
				}
				cli_len = sizeof addr;
				nsfd = accept(sfd[i], (struct sockaddr*)&addr, &cli_len);
				if(nsfd < 0) {
					perror("accept() ");
					continue;
				}
				if(nusfd[i] == 0) {
					nusfd[i] = sock_accept(usfd[i]);
					if(nusfd < 0) {
						perror("sock_accept() ");
						continue;
					}
				}
				if(send_fd(nusfd[i], nsfd) < 0) {
					perror("send_fd() ");
				}
				close(nsfd);
			}
		}
	}
	return 0;
}