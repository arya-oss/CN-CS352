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

#define BUFSIZE 1024
#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main(int argc, char const *argv[]) {
    int port, serv_no; char buf[128];
    if (argc == 1){
        eerror("Usage: ./client <port>");
    } else {
        port = atoi(argv[1]);
        if(port < 1024 || port > 65535) 
            eerror("Port must between 1024 to 65535");
    }
    struct sockaddr_in s_addr;
    char buffer[BUFSIZE]; int sfd, n;
    memset((char *) &s_addr, 0, sizeof(s_addr));

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        eerror("socket() error");

    if (connect(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("connect() error");

    printf("connection successfull\n");
    
   // while(1) {
        read(0, buf, 128);
        if(write(sfd, buf, strlen(buf)) < 0)
            eerror("send() error")
        //if(strcmp(buf, "exit") == 0)
        //    break;
        if((n=read(sfd, buffer, 128)) < 0)
            eerror("recv() error");
        write(1, buffer, n);
   // }

    printf("\nExiting ...\n");
    close(sfd);

    return 0;
}
