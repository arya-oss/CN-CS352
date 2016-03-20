#include "utils.h"

#define BUFSIZE 1024

void * runner(void * args) {
	int sfd = *(int *)args; char buf[BUFSIZE];
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
}

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		printf("Usage: ./client <port>\n");
		exit(1);
	}
	pthread_t pid;
	int port = atoi(argv[1]);
	int sfd = tcpsocket_connect(port);
	if(sfd < 0) {
		perror("tcpclient_connect() ");
		exit(2);
	}
	char buf[BUFSIZE]; int id; ticket_t tkt;
	recv(sfd, buf, BUFSIZE, 0);
	if(atoi(buf) == -1) {
		printf("Housefull Oops !!\n");
		close(sfd);
	} else {
		printf("%s\n", buf);
		memset(buf, 0, BUFSIZE);
		scanf("%d", &id);
		sprintf(buf, "%d", id);
		send(sfd, buf, strlen(buf), 0);
		memset(buf, 0, BUFSIZE);
		recv(sfd, buf, BUFSIZE, 0);
		printf("Ticket ID: %s", buf);
		sleep(1); // wait for final confirmation
		memset(buf, 0, BUFSIZE);
		recv(sfd, buf, BUFSIZE, 0);
		if(atoi(buf) == -1) {
			printf("Housefull Oops !! Your Ticket has been canceled !!\n");
			close(sfd);
			exit(3);
		} else {
			printf("Enjoy !! You got entered in %s\n", buf);
			pthread_create(&pid, NULL, &runner, (void *) &sfd);
			pthread_join(pid, NULL);
		}
	}
	return 0;
}