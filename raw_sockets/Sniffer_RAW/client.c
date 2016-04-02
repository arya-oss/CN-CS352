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
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>

#define BUFSIZE 1024

int main(int argc, char const *argv[]) {
    struct sockaddr_in addr; int rsfd, n=1, len; char buffer[BUFSIZE];
    rsfd = socket(AF_INET, SOCK_RAW, 146);
    if(rsfd < 0) {
        perror("socket() ");
        exit(1);
    }
    memset((char *)&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = argc == 1? 8001:atoi(argv[1]);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    while(1) {
        if(sendto(rsfd, "Rajmani Arya", 13, 0, (struct sockaddr *)&addr, sizeof addr) < 0) {
            perror("sendto() ");
        }
	printf("Packet Sent %d \n", n++);
        sleep(3);
    }
    return 0;
}
