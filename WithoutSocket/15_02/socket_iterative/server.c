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
    // struct hostent * h_ptr; struct protoent * p_ptr; 
    struct sockaddr_in s_addr, c_addr;
    char buffer[BUFSIZE]; int sfd, _sfd, cli_len, n, i;
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
    int nClient=0;
    while(1) {
        cli_len = sizeof(_sfd);
        
        if((_sfd = accept(sfd, (struct sockaddr *) &_sfd, &cli_len)) < 0)
            eerror("accept() error");
        nClient++;
        n = read(_sfd, buffer, BUFSIZE);

        write(1, buffer, n);
        
        for(i=0; i<n; i++)
            if(islower(buffer[i]))
                buffer[i] = buffer[i]-32;
        
        if(write(_sfd, buffer, n) < 0 )
            eerror("write() error");
        printf("\nClient %d disconnented.\n", nClient);
        close(_sfd);
    }
    return 0;
}
