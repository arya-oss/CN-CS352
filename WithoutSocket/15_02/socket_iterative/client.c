#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define port 9503
#define BUFSIZE 1024
#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main(int argc, char const *argv[]) {
    // struct hostent * h_ptr; struct protoent * p_ptr; s
    struct sockaddr_in s_addr, c_addr;
    char buffer[BUFSIZE]; int sfd, n;
    memset((char *) &s_addr, 0, sizeof(s_addr));
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        eerror("socket() error");

    if (connect(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0) 
        eerror("connect() error");

    if(write(sfd, "hello", 5) < 0)
        eerror("send() error")
    if((n=read(sfd, buffer, 1024)) < 0)
        eerror("recv() error");
    
    write(1, buffer, n);

    printf("Exiting ...\n");
    close(sfd);

    return 0;
}
