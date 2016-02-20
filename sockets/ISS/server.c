#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define size 128
#define MAX 4
#define eerror(msg) { printf("%s\n", msg); exit(1); }

typedef struct service
{
    int port;
    char * s_name;
    int id;
}service_t;

service_t serv[] = { {9501, "s1", 1}, {9502, "s2", 2}, {9503, "s3", 3}, {9504, "s4", 4} };

int main(int argc, char const *argv[]) {
    struct sockaddr_in s_addr, c_addr;

    int sfds[MAX], _sfd, cli_len, n, i, port; char buf[size];

    for (i=0; i<MAX; i++) {
        if ((sfds[i]=socket(AF_INET, SOCK_STREAM, 0)) == -1)
            eerror("socket() error");

        memset((char *) &s_addr, 0, sizeof(s_addr));

        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons((u_short) serv[i].port);
        s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if( bind(sfds[i], (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
            eerror("bind() error");

        if(listen(sfds[i], 3) < 0)
            eerror("listen() error");
        printf("Server Listening on port %d ...\n", serv[i].port);
    }
    fd_set rfds; int max;

    while(1) {
        FD_ZERO(&rfds); max=-1;
        for (i = 0; i < MAX; ++i){
            FD_SET(sfds[i], &rfds);
            if(sfds[i] > max) max = sfds[i];
        }
        int res = select(max+1, &rfds, NULL, NULL, NULL);
        if(res < 0) continue;
        for (i = 0; i < MAX; ++i){
            if(FD_ISSET(sfds[i], &rfds)) {
                if((_sfd=accept(sfds[i], (struct sockaddr *)&c_addr, &cli_len)) < 0)
                    eerror("accept() error");
                if(fork() == 0) {
                    dup2(_sfd, 0);
                    dup2(_sfd, 1);
                    execl(serv[i].s_name, serv[i].s_name, NULL);
                    eerror("execl() error");
                }
            }
        }
    }

    return 0;
}
