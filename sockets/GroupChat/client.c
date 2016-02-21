#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define eerror(msg) { printf("%s\n", msg); exit(1); }

void * writer(void * args);
void * reader(void * args);

int main(int argc, char const *argv[]) {
    int port, serv_no; char buf[128]; pthread_t r,w;
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
        eerror("socket() error1");

    if (connect(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("connect() error1");
    
    printf("Type Group Id (port) : ");
    scanf("%d", &serv_no);
    sprintf(buf, "%d", serv_no);
    write(sfd, buf, strlen(buf));
    read(sfd, buf, 128);
    close(sfd);
    if(strcmp(buf, "exit") == 0) 
        eerror("Service Unavailable !");

    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        eerror("socket() error2");

    memset((char *) &s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) serv_no);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sleep(1);
    if (connect(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("connect() error2");

    printf("connection successfull\n");
    
    pthread_create(&r, NULL, * reader, (void *)&sfd);
    pthread_create(&w, NULL, * writer, (void *)&sfd);

    pthread_join(r, NULL);
    pthread_join(w, NULL);

    printf("\nExiting ...\n");
    close(sfd);

    return 0;
}

void * reader(void * args) {
    char buf[BUFSIZE];
    int sfd = *(int *) args;
    while(1) {
        read(sfd, buf, BUFSIZE);
        printf("%s", buf);
    }
}

void * writer(void * args) {
    char buf[BUFSIZE];
    int n,sfd = *(int *) args;
    while(1) {
        fgets(buf, BUFSIZE, stdin);
        write(sfd, buf, BUFSIZE);
        if (atoi(buf) == -1) {
            eerror("Exiting...");
        }
    }
}