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
#define MAX 10
#define eerror(msg) { printf("%s\n", msg); exit(1); }

volatile int nClient=1; char buffer[BUFSIZE], buf[BUFSIZE]; 

int main(int argc, char const *argv[]) {
    struct sockaddr_in s_addr, c_addr;
    int sfds[MAX], _sfd, cli_len, i, len, j, n, port, max;
    fd_set rfds;
    if(argc == 1) {
        eerror("Usage: ./service <port>")
    } else
        port = atoi(argv[1]);

    memset((char *) &s_addr, 0, sizeof(s_addr));
    
    if ((sfds[0]=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if( bind(sfds[0], (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfds[0], 5) < 0)
        eerror("listen() error");
    
    printf("Group started on port %d ...\n", port);

    while(1) {
        FD_ZERO(&rfds);
        for (i = 0; i < nClient; ++i){
            FD_SET(sfds[i], &rfds);
            if(sfds[i] > max) max = sfds[i];
        }
        int res = select(max+1, &rfds, NULL, NULL, NULL);
        if(res < 0) continue;
        cli_len = sizeof(c_addr);
        len = nClient;
        for (i = 0; i < len; ++i) {
            if(FD_ISSET(sfds[i], &rfds)) {
                if(i == 0) {
                    if((sfds[nClient] = accept(sfds[0], (struct sockaddr *) &c_addr, &cli_len)) < 0)
                        eerror("accept() error");
                    printf("Client %d connected. on Group %d!!\n", nClient, port);
                    nClient++;
                } else {
                    memset(buf, 0, BUFSIZE);
                    n = read(sfds[i], buf, BUFSIZE);
                    // printf("%d %d", i, n);
                    if(strlen(buf) <= 1) continue;
                    if(atoi(buf) == -1) {
                        close(sfds[i]);
                        sfds[i] = -1;
                        continue;
                    }
                    memset(buffer, 0, BUFSIZE);
                    sprintf(buffer, "P%d: %s", i, buf);
                    for(j=1; j < len; j++) {
                        if(i!=j)
                            write(sfds[j], buffer, strlen(buffer));
                    }
                }
            }
        }
    }
    return 0;
}