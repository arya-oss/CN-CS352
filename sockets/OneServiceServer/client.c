/*
 * Connection Less (UDP Client)
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
volatile int sfd, nsfd, override;

void * listener(void * args) {
	printf("client listener started !\n");
	struct sockaddr_in c_addr; int cli_len = sizeof(c_addr); char _tmp[128];
	if(listen(sfd, 5) < 0) {
		eerror("listen() error");
	}
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds); FD_SET(sfd, &rfds);
		if(select(sfd+1, &rfds,  NULL, NULL, NULL) < 0) {
			sleep(0.1);
			continue;
		}
		if(FD_ISSET(sfd, &rfds)) {
			if((nsfd = accept(sfd, (struct sockaddr *)&c_addr, &cli_len)) < 0){
				eerror("accept() error");
			}
		}
		read(nsfd, _tmp, 128);
		printf("%s\n", _tmp);
		if(override) {
			write(nsfd, "1", 1);
			write(sfd, "-1", 2);
			printf("permission granted exiting...\n");
			close(sfd);
			exit(1);
		} else {
			write(nsfd, "-1", 2);
			close(nsfd);
		}
	}
}

void * runner(void * args) {
	int fd = *(int *)args; char _tmp[128], c;
	printf("service started !\n");
	while(1) {
		memset(_tmp, 0, 128);
		scanf("%s", _tmp); scanf("%c", &c);
		write(fd, _tmp, strlen(_tmp));
		read(fd, _tmp, 128);
		printf("%s\n", _tmp);
	}
}

int main(int argc, char * argv[]) {
	int port;
	if(argc < 3) {
		eerror("Usage: ./client <port> <allow_override 0/1>");
	}
	override = atoi(argv[2]);
	port = atoi(argv[1]);
	pthread_t pid;
	struct sockaddr_in s_addr, c_addr; int i; char buf[BUFSIZE];
	memset((char *)&s_addr, 0, sizeof(s_addr));
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1) {
		eerror("socket() error");
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons((u_short) port);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(sfd, (struct sockaddr *)&s_addr, sizeof s_addr) < 0){
		eerror("connect() error1");
	}

	recv(sfd, buf, BUFSIZE, 0);

	if(atoi(buf) == 1) {
		// connection established with server
		pthread_create(&pid, NULL, &runner, (void *)&sfd);
		pthread_join(pid, NULL);
	} else {
		// take permission from currently connected client
		int _sfd = socket(AF_INET, SOCK_STREAM, 0);
		memset((char *)&c_addr, 0, sizeof c_addr);
		c_addr.sin_family = AF_INET;
		c_addr.sin_port = atoi(buf); // replied port from server
		c_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		if(connect(_sfd, (struct sockaddr *)&c_addr, sizeof c_addr) < 0){
			eerror("connect() error2");
		}
		write(_sfd, "May I connect to server ?", 25);
		memset(buf, 0, BUFSIZE);
		read(_sfd, buf, BUFSIZE);
		close(_sfd);
		if(atoi(buf) == 1) {
			pthread_create(&pid, NULL, &runner, (void *)&sfd);
			pthread_join(pid, NULL);
		} else {
			close(_sfd);
		}
	}
	pthread_t lPid;
	pthread_create(&lPid, NULL, &listener, NULL);
	pthread_join(lPid, NULL);
	return 0;
}

