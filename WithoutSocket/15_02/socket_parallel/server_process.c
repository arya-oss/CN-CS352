#include <stdio.h>
#include <stdlib.h>
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

volatile int nClient=0; char buffer[BUFSIZE]; 

int main(int argc, char const *argv[]) {
    // struct hostent * h_ptr; struct protoent * p_ptr; 
    struct sockaddr_in s_addr, c_addr;
    int sfd, _sfd, cli_len, n;
    memset((char *) &s_addr, 0, sizeof(s_addr));
    
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    if( bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfd, 5) < 0)
        eerror("listen() error");
    
    printf("Server Listening on port %d ...\n", port);
    
    while(1) {
        cli_len = sizeof(_sfd);
        if((_sfd = accept(sfd, (struct sockaddr *) &_sfd, &cli_len)) < 0)
            eerror("accept() error");
        if(fork() == 0) {
            close(sfd);
            dup2(_sfd, 0);
            dup2(_sfd, 1);
            execlp("./service", "./service", NULL);
            printf("execlp() error\n");
        }
        nClient++;
        printf("Client %d connected. !!\n", nClient);
    }
    return 0;
}
