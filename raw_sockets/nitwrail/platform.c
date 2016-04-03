#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <poll.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
/* contains all useful functions */
#include "utils.h"

int usfd, tsfd, id, s_pid;
char buf[128];

int getPIDfromUsfd(int _usfd){
	struct ucred cred;
	int len = sizeof(cred);
	if(getsockopt(_usfd, SOL_SOCKET, SO_PEERCRED, &cred, &len) < 0) {
		perror("getsockopt() ");
	}
	return cred.pid;
}

void * advertisment(void * args) {
	int rsfd = socket(AF_INET, SOCK_RAW, 148);
	if(rsfd < 0) {
		perror("socket() ");
		pthread_exit(0);
	}
	char buf[1024]; struct sockaddr_in addr; int len = sizeof addr;
	struct iphdr * ip;
	while(1) {
		if(recvfrom(rsfd, buf, 1024, 0, (struct sockaddr*)&addr, &len) < 0) {
			perror("recvfrom() ");
			continue;
		}
		ip = (struct iphdr *)buf;
		int iphdrlen = ip->ihl*4;
		char * message = buf+iphdrlen;
		printf("Ad: %s\n", message);
	}
}

void * announcement(void * args) {
	int rsfd = socket(AF_INET, SOCK_RAW, 146);
	if(rsfd < 0) {
		perror("socket() ");
		pthread_exit(0);
	}
	char buf[1024]; struct sockaddr_in addr; int len = sizeof addr;
	struct iphdr * ip;
	while(1) {
		if(recvfrom(rsfd, buf, 1024, 0, (struct sockaddr*)&addr, &len) < 0) {
			perror("recvfrom() ");
			continue;
		}
		ip = (struct iphdr *)buf;
		int iphdrlen = ip->ihl*4;
		char * message = buf+iphdrlen;
		printf("%s\n", message);
	}

}

int main(int argc, char const *argv[])
{
	char buf[128]; pthread_t pid1,pid2;
	if(argc < 2){
		printf("Usage: \n\t ./platform [1|2|3]\n");
		exit(1);
	}
	id = atoi(argv[1])-1;

	usfd = unixsock_connect("/tmp/sockpath");
	if(usfd < 0) {
		perror("unixsock_connect() ");
		exit(1);
	}
	s_pid = getPIDfromUsfd(usfd);
	pthread_create(&pid1, NULL, &advertisment, NULL); // thread for advertisment
	pthread_create(&pid2, NULL, &announcement, NULL); // thread for incoming train announcements
	while(1) {
		tsfd = recv_fd(usfd);
		if(tsfd < 0){
			perror("recv_fd() ");
			continue;
		}
		while(1) {
			memset(buf, 0, 128);
			recv(tsfd, buf, 128, 0);
			if(atoi(buf) == -1) {
				printf("\nTrain Sequence Completed !\n");
				close(tsfd);
				kill(s_pid, SIGUSR1);
				break;
			}
			printf("%s ", buf);
		}
	}
	return 0;
}
