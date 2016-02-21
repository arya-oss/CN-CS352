#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFSIZE 128
#define eerror(msg) { printf("%s\n", msg); exit(1); }

void * runner(void * args);

volatile int nClient=0; char buffer[BUFSIZE]; 


int main(int argc, char const *argv[]) {
    struct sockaddr_in s_addr, c_addr;
    int sfd, _sfd, cli_len, i, n, port;
        
    if(argc == 1) {
        eerror("Usage: ./service <port>")
    } else
        port = atoi(argv[1]);

    memset((char *) &s_addr, 0, sizeof(s_addr));
    
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if( bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfd, 5) < 0)
        eerror("listen() error");
    
    printf("Service Listening on port %d ...\n", port);
    pthread_t pid;
    while(1) {
        cli_len = sizeof(c_addr);
        if((_sfd = accept(sfd, (struct sockaddr *) &c_addr, &cli_len)) < 0)
            eerror("accept() error");
        pthread_create(&pid, NULL, &runner, (void *) &_sfd);
        nClient++;
        printf("Client %d connected. !!\n", nClient);
    }
    return 0;
}

void * runner (void * args) {
    int i,sfd = *(int *) args; char buf[BUFSIZE];
    while(1) {
        memset(buf, '\0', BUFSIZE);
        read(sfd, buf, BUFSIZE);
        if(strcmp(buf, "exit") == 0)
            break;
        for (i = 0; i < strlen(buf); ++i) {
            if(islower(buf[i]))
                buf[i] = buf[i]-32;
        }
        write(sfd, buf, strlen(buf));
    }
}