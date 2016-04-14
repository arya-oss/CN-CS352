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
#include <netdb.h>
#include <netinet/if_ether.h>

#define BUFSIZE 65536

/**
 * Title: TCP Header and data print
 * description: catch ip_packets and print tcp header and payload data
 *
 * Compile:
 *		gcc tcpheader.c -o tcpheader
 * RUN:
 *	sudo ./tcpheader
 */

int main(int argc, char * argv[]) {
	struct sockaddr_in s_addr, d_addr;
	int rsfd, n, len; char buf[BUFSIZE]; char addr[32];
	rsfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(rsfd < 0) {
		perror("socket() ");
		exit(1);
	}
	
	len = sizeof s_addr;
	struct iphdr * iph; unsigned short iphdrlen;
	struct tcphdr * th;
	int i=0;
	while(1) {
		n = recvfrom(rsfd, buf, BUFSIZE, 0, (struct sockaddr*)&s_addr, &len);
		if(n < 0) {
			perror("recvfrom() ");
		}
		iph = (struct iphdr *) (buf+sizeof(struct ethhdr));
		memset((char *)&s_addr, 0, sizeof s_addr);
		memset((char *)&d_addr, 0, sizeof d_addr);
		s_addr.sin_addr.s_addr = iph->saddr;
		d_addr.sin_addr.s_addr = iph->daddr;
		
		iphdrlen = iph->ihl*4;
		
		/*printf("------------- IP Header ------------\n");
		printf("|%4d|%4d|%8d|%16d|\n", (unsigned int)iph->version, (unsigned int)iph->ihl, (unsigned int)iph->tos, ntohs(iph->tot_len));
		printf("------------------------------------\n");
		printf("|%13d|R|D|M|%13d|\n", ntohs(iph->id), (unsigned int)iph->frag_off);
		printf("------------------------------------\n");
		printf("|%8d|%8d|%16d|\n", (unsigned int)iph->ttl, (unsigned int)iph->protocol, ntohs(iph->check));
		printf("------------------------------------\n");
		printf("%s\n", inet_ntoa(s_addr.sin_addr));
		printf("------------------------------------\n");
		printf("%s\n", inet_ntoa(d_addr.sin_addr));
		printf("------------------------------------\n");*/
		if(iph->protocol != 6) continue;
		th = (struct tcphdr *) (buf+iphdrlen+sizeof(struct ethhdr));
		printf("------------- TCP HEADER -----------\n");
		printf("%10d|%10d\n", ntohs(th->source), ntohs(th->dest));
		printf("------------------------------------\n");
		printf("%10d\n" , ntohl(th->seq));
		printf("------------------------------------\n");
		printf("%10d\n" , ntohl(th->ack_seq));
		printf("------------------------------------\n");
		printf("%4d|  |%d|%d|%d|%d|%d|%d|%10d|\n" , th->doff, th->fin, th->syn, th->rst, th->psh, th->ack, th->urg, ntohs(th->window));
		printf("------------------------------------\n");
		printf("%10d|%10d\n" , ntohs(th->check), th->urg_ptr);
		printf("------------------------------------\n");
		printf("-------------- PayLoad -------------\n");
		printf("%s\n", buf+iphdrlen+sizeof(struct ethhdr)+4*th->doff);
	}
	return 0;
}
