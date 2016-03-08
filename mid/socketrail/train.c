#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <unistd.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main(int argc, char const *argv[])
{
	if(argc < 3) 
		eerror("Usage: \n\t ./train <port> <bogey sequence>\n");
	struct sockaddr_in s_addr;
    int sfd, _sfd, port; char buf[128];
    port = atoi(argv[1]);

    memset((char *) &s_addr, 0, sizeof(s_addr));
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("connect() error");
    read(sfd, buf, 128);
    port = atoi(buf); // getting port no. of platform from server
    close(sfd);
    printf("Got response %s\n", buf);
    if(port == -1) {
    	printf("Platform not available! Try again !\n");
    } else {
    	memset((char *) &s_addr, 0, sizeof(s_addr));
	    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
	        eerror("socket() error");

	    s_addr.sin_family = AF_INET;
	    s_addr.sin_port = htons((u_short) port);
	    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        printf("connecting again !\n");
	    if (connect(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
	        eerror("connect() error");
    	write(sfd, argv[2], strlen(argv[2]));
        printf("written sequence\n");
        sleep(2);
    	write(sfd, "-1", 2);
    }
    sleep(2);
    close(sfd);
    printf("Leaving !\n");
	return 0;
}
