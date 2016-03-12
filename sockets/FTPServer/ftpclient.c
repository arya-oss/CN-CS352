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
char buf[BUFSIZE];
/**
 * Author: Rajmani Arya
 * Date: 11th March 2016
 */

int main(int argc, char * argv[]) {
	int port;
	if(argc < 4) {
		printf("Usage: ./client <port> <src_file> <dest_file\n");
		exit(1);
	}
	port = atoi(argv[1]);

	struct sockaddr_in s_addr; int cli_len, sfd, n, i=1;
	memset((char *)&s_addr, 0, sizeof(s_addr));
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1) {
		eerror("socket() error");
	}
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons((u_short) port);
	s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(sfd, (struct sockaddr *)&s_addr, sizeof s_addr) < 0){
		eerror("connect() error");
	}
	write(sfd, argv[2], strlen(argv[2])); // Request for file

	read(sfd, buf, BUFSIZE);
	// printf("%s\n", buf);
	if(atoi(buf) == -1) {
		printf("Invalid filename ! try again !\n");
		exit(1);
	}
	FILE * f = fopen(argv[3], "w");
	if(!f) {
		eerror("target filename open() error");
	}
	printf("starting\n"); fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		FD_SET(sfd,  &rfds);
		if(select(sfd+1, &rfds, NULL, NULL, NULL) < 0) {
			sleep(0.2);
			continue;
		}
		if(FD_ISSET(sfd, &rfds)) {
			n = read(sfd, buf, BUFSIZE);
			if(n == 2 || strcmp(buf, "-1") == 0) {
				break;
			}
			// printf("%s\n", buf);
			fprintf(f, "%s", buf);
			printf("Part %d completed !\n", i++);
			memset(buf, 0, BUFSIZE);
		}
	}
	fclose(f);
	printf("%s transfer completed to %s !\n", argv[2], argv[3]);
	close(sfd);
	return 0;
}
