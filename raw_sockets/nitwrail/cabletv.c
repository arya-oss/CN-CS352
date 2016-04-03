#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <poll.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

int main() {
    int rsfd;
    rsfd = socket(AF_INET, SOCK_RAW, 148); // initialize raw socket on protocol 148
	if(rsfd < 0) {
		perror("socket() ");
	}
    /* making sockaddr_in for Advertisments */
    struct sockaddr_in addr;
    memset((char *)&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    char buf[128];
    while(1) {
        printf("Enter Ad: ");
        scanf("%s", buf);
        if(sendto(rsfd, buf, strlen(buf), 0, (struct sockaddr*)&addr, sizeof addr) < 0){
            perror("sendto() ");
        }
    }
    return 0;
}
