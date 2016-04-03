#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "utils.h"

#define BUFSIZE 1024

int main(int argc, char const *argv[]) {
    int port, sfd; char buf[128];
    if(argc < 2) {
        printf("Usage: ./client <port no>\n");
        exit(1);
    }
    port = atoi(argv[1]);
    sfd = tcpsock_connect("127.0.0.1", port);
    if(sfd < 0) {
        perror("tcp_connect() ");
        exit(1);
    }

    memset(buf, 0, 128);
    recv(sfd, buf, 128, 0);
    if(atoi(buf) == -1) {
        close(sfd);
        printf("Platform Busy !!\n");
        exit(1);
    }

    printf("Got %s\n", buf);

    while(1) {
        printf("Enter bogey seq (-1 to exit): ");
        memset(buf, 0, 128);
        scanf("%s", buf);
        if(atoi(buf) == -1) {
            send(sfd, "-1", 2, 0);
            close(sfd);
            break;
        }
        send(sfd, buf, strlen(buf), 0);
    }
    printf("Train Leaved Platform\n");
    return 0;
}
