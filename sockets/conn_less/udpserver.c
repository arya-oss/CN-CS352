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

int main(int argc, char * argv[]) {
	int port;
	if(argc == 1) {
		port = 9500;
	} else {
		port = atoi(argv[1]);
	}
	struct sockaddr_in s_addr, c_addr; int cli_len, sfd, nsfd, i;
	char buf[BUFSIZE];
	memset((char *)&s_addr, 0, sizeof(s_addr));
	sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sfd == -1) {
		eerror("socket() error");
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons((u_short) port);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(sfd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0){
		eerror("bind() error");
	}
	for (i = 0; i < 10; ++i) {
		cli_len = sizeof(c_addr);
		if(recvfrom(sfd, buf, BUFSIZE, 0, (struct sockaddr*)&c_addr, &cli_len) == -1) {
			eerror("recvfrom() error");
		}
		printf("%s\n", buf);
		memset(buf, 0, BUFSIZE);
		// printf("Recieved Data From %s:%d\nData: %s\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port), buf);
		strcpy(buf, "welcome client");
		if(sendto(sfd, buf, strlen(buf), 0, (struct sockaddr*)&c_addr, sizeof(c_addr)) == -1) {
			eerror("sendto() error");
		}
		memset(buf, 0, BUFSIZE);
	}
	close(sfd);
	return 0;
}
