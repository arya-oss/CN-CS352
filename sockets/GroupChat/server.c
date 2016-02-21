#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define size 128
#define MAX 3
#define SHMKEY 137146
#define eerror(msg) { printf("%s\n", msg); exit(1); }

typedef struct group {
    int port;
    char * g_name;
    int id;
} group_t;

group_t grp[MAX] = { {9501, "s1", 1}, {9502, "s2", 2}, {9503, "s3", 3} };

int get_grp_id(int port) {
    int i;
    for (i = 0; i < MAX; ++i){
        if(grp[i].port == port)
            return grp[i].id;
    }
    return i+1;
}

int clients[MAX] = {0};

int main(int argc, char const *argv[]) {
    struct sockaddr_in s_addr, c_addr;
    int sfd, _sfd, cli_len, n, i, port; char buf[size];
    if(argc == 1){
        port = 9500;
    } else {
        port = atoi(argv[1]);
        if(port < 1024 || port > 16383)
            eerror("Usage: ./server [portno]");
    }

    memset((char *) &s_addr, 0, sizeof(s_addr));
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if( bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfd, 3) < 0)
        eerror("listen() error");
    printf("Super Server Listening on port %d ...\n", port);
    fd_set rfds;

    // while(1) {
    start:
        FD_ZERO(&rfds);
        FD_SET(sfd, &rfds);
        if (select(sfd+1, &rfds, NULL, NULL, NULL) > 0) {
            if(!FD_ISSET(sfd, &rfds))
                goto start;

            cli_len = sizeof(c_addr);
            if((_sfd=accept(sfd, (struct sockaddr *)&c_addr, &cli_len)) < 0){
                eerror("accept() error");
            }
            read(_sfd, buf, size);
            
            int serv_id = get_grp_id(atoi(buf));

            if(serv_id > MAX) {
                write(_sfd, "exit", 4);
                goto start;
            } else {
                write(_sfd, "go", 2);
                sprintf(buf, "%d", grp[serv_id-1].port);
                if(clients[serv_id-1] == 0) {
                    clients[serv_id-1]++;
                    if(fork() == 0) {
                        execl(grp[serv_id-1].g_name, grp[serv_id-1].g_name, buf, NULL);
                        eerror("execl() error");
                    }
                }
                printf("%d\n", clients[serv_id-1]);  
            }
        }
        goto start;
    // }

    return 0;
}
