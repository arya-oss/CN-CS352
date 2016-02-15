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

#define port 9503
#define BUFSIZE 1024
#define eerror(msg) { printf("%s\n", msg); exit(1); }

volatile int nClient=0; char buffer[BUFSIZE]; 

void * run_client(void * args) {
    int sfd = *(int * )args;
    int i,n, * ret;
    n = read(sfd, buffer, BUFSIZE);
    write(1, buffer, n);
        
    for(i=0; i<n; i++)
        if(islower(buffer[i]))
            buffer[i] = buffer[i]-32;
        
    if(write(sfd, buffer, n) < 0 )
        eerror("write() error");
    printf("\nClient %d disconnented.\n", nClient);
    close(sfd);
    pthread_exit(ret);
}

int main(int argc, char const *argv[]) {
    // struct hostent * h_ptr; struct protoent * p_ptr; 
    struct sockaddr_in s_addr, c_addr;
    int sfd, _sfd, cli_len;
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
    
    printf("Server Listening ...\n");
    int cl_sfds[100]; pthread_t pid;
    while(1) {
        cli_len = sizeof(_sfd);
        if((cl_sfds[nClient] = accept(sfd, (struct sockaddr *) &_sfd, &cli_len)) < 0)
            eerror("accept() error");
        pthread_create(&    pid, NULL, &run_client, (void *)&(cl_sfds[nClient]));
        nClient++;
    }
    return 0;
}
