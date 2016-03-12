/*
 * File Transfer Server
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

void * copy_worker(void * args) {
	int n, nsfd = *(int *) args;
	char filename[BUFSIZE];
	read(nsfd, filename, BUFSIZE);
	int fd = open(filename, O_RDONLY);
	if(fd < 0) {
		printf("Invalid Filename\n");
		write(nsfd, "-1", BUFSIZE); // send acknowledgement abort
		close(nsfd);
		pthread_exit(0);
	} else {
		printf("Valid Filename\n");
		write(nsfd, "ok", BUFSIZE); //send acknowledgement proceed
	}
	int i=1;
	while((n = read(fd, buf, BUFSIZE)) > 0) {
		write(nsfd, buf, n);
		printf("Part %d sent.\n", i++);
		memset(buf, 0, BUFSIZE);
	}
	sleep(1);
	write(nsfd, "-1", 2);
	printf("Completed !! %s\n", filename);
	close(nsfd);
	pthread_exit(0);
}

int main(int argc, char * argv[]) {
	int port;
	if(argc == 1) {
		port = 9500;
	} else {
		port = atoi(argv[1]);
	}
	struct sockaddr_in s_addr, c_addr; int cli_len, sfd, nsfd, i;
	pthread_t pid;
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
	printf("Server listening !\n");
	while(1) {
		cli_len = sizeof(c_addr);
		nsfd = accept(sfd, (struct sockaddr *)&c_addr, &cli_len);
		if(nsfd < 0) {
			eerror("accept() error");
		}
		pthread_create(&pid, NULL, &copy_worker, (void *) &nsfd);
	}
	return 0;
}
