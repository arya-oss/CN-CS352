/*
 * Connection Less (UDP Server)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define eerror(msg) printf("%s\n", msg),exit(1);
/**
 * Author: Rajmani Arya
 * Date: 11th March 2016
 */
volatile int available = 1; // 1 => available , 0 => busy
volatile int curr_client_port = -1; // stores current client port

void * service (void * args) {
	int i, fd = *(int *) args; char _tmp[128];
	while(1) {
		read(fd, _tmp, 128);
		printf("%s\n", _tmp);
		if(atoi(_tmp) == -1) {
			available = 1;
			curr_client_port = 0;
			pthread_exit(0);
		}
		for (i = 0; i < strlen(_tmp); ++i){
			if (_tmp[i] >= 'a' && _tmp[i] <= 'z'){
				_tmp[i] = (int)_tmp[i]-32;
			}
		}
		write(fd, _tmp, strlen(_tmp));
	}
}

int main(int argc, char * argv[]) {
	int port = (argc == 1) ? 9500 : atoi(argv[1]);

	pthread_t pid;
	struct sockaddr_in s_addr, c_addr; int cli_len, sfd, nsfd, i;
	char buf[BUFSIZE];
	memset((char *)&s_addr, 0, sizeof(s_addr));
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1) {
		eerror("socket() error");
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons((u_short) port);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(sfd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0){
		eerror("bind() error");
	}
	if(listen(sfd, 5) < 0) {
		eerror("listen() error");
	}
	while(1) {
		cli_len = sizeof(c_addr);
		nsfd = accept(sfd, (struct sockaddr *)&c_addr, &cli_len);
		if(nsfd < 0) {
			eerror("accept() error");
		}
		getpeername(nsfd, (struct sockaddr *)&s_addr, &cli_len);
		printf("Client address IP:Port => %s:%d\n", inet_ntoa(s_addr.sin_addr), ntohs(s_addr.sin_port));
		if(available) {
			write(nsfd, "1", 1);
			curr_client_port = ntohs(s_addr.sin_port);
			available = 0; // make server busy
			pthread_create(&pid, NULL, &service, (void *)&nsfd);
		} else {
			char _tmp[32];
			sprintf(_tmp, "%d", (int)ntohs(s_addr.sin_port));
			write(nsfd, _tmp, strlen(_tmp));
			sleep(1); // wait of some time for conversation b.w clients
			read(nsfd, _tmp, 32);
			if(atoi(_tmp) == 1) {
				if(available) {
					getpeername(nsfd, (struct sockaddr *)&s_addr, &cli_len);
					curr_client_port = ntohs(s_addr.sin_port);
					available = 0; // make server busy
					pthread_create(&pid, NULL, &service, (void *)&nsfd);
				} else {
					close(nsfd);
				}
			} else {
				close(nsfd);
			}
		}
	}
	return 0;
}
// end of server.c
