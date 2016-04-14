#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netdb.h>
#include <netinet/if_ether.h>
#include <pcap.h>
#include <sys/time.h>

// #define APP_DATA

/**
 * Title: Packet Sniffer
 * Description: catch any n/w packet filter IP Packets and print IP Header
 * if IP_PROTOCOL is either TCP or UDP then print corresponding header
 * for printing payload data just uncomment #deine APP_DATA written above.
 * 
 * for header field info check.
 * ethernet header -> /usr/include/net/ethernet.h
 * ip header -> /usr/include/netinet/ip.h
 * tcp header -> /usr/include/netinet/tcp.h
 * udp header -> /usr/include/netinet/udp.h
 * 
 * Dependancies: libpcap , to install -> sudo apt-get install libpacp-dev
 * Compile:
 * 		gcc all_header.c -o sniffer -lpcap
 *
 * RUN: (for catching 100 packets)
 *		sudo ./sniffer 100 
 * 
 * Author: Rajmani Arya
 * Date: 14th Apr 2016
 * NOTE: connection require (either wifi or lan)
 */

void print_ip_header(const struct iphdr * iph){
	struct sockaddr_in s_addr, d_addr;
	s_addr.sin_addr.s_addr = iph->saddr;
	d_addr.sin_addr.s_addr = iph->daddr;
		
	printf("\n------------- IP Header ------------\n");
	printf("|%4d|%4d|%8d|%16d|\n", (unsigned int)iph->version, (unsigned int)iph->ihl, (unsigned int)iph->tos, ntohs(iph->tot_len));
	printf("------------------------------------\n");
	printf("|%16d|R|D|M|%12d|\n", ntohs(iph->id), (unsigned int)iph->frag_off);
	printf("------------------------------------\n");
	printf("|%8d|%8d|%16d|\n", (unsigned int)iph->ttl, (unsigned int)iph->protocol, ntohs(iph->check));
	printf("------------------------------------\n");
	printf("%s\n", inet_ntoa(s_addr.sin_addr));
	printf("------------------------------------\n");
	printf("%s\n", inet_ntoa(d_addr.sin_addr));
	printf("------------ IP Header End ---------\n");
}

void print_tcp_header(const struct tcphdr * th){
	printf("\n------------- TCP HEADER -----------\n");
	printf("%16d|%16d\n", ntohs(th->source), ntohs(th->dest));
	printf("------------------------------------\n");
	printf("%32d\n" , ntohl(th->seq));
	printf("------------------------------------\n");
	printf("%32d\n" , ntohl(th->ack_seq));
	printf("------------------------------------\n");
	printf("%4d| |%d|%d|%d|%d|%d|%d|%16d|\n" , th->doff, th->fin, th->syn, th->rst, th->psh, th->ack, th->urg, ntohs(th->window));
	printf("------------------------------------\n");
	printf("%16d|%16d\n" , ntohs(th->check), th->urg_ptr);
	printf("----------- TCP Header End ---------\n");
}

void print_udp_header(const struct udphdr * uh){
	printf("\n------------- UDP HEADER --------\n");
	printf("%16d|%16d\n", ntohs(uh->source), ntohs(uh->dest));
	printf("-----------------------------------\n");
	printf("%16d|%16d\n" , (unsigned int)uh->len, ntohs(uh->check));
	printf("----------- UDP Header End --------\n");
}

void callback(unsigned char *no_use, const struct pcap_pkthdr * phdr, const unsigned char * packet) {
	static int cnt_pkt=1;
	struct ether_header * ehdr;
	struct iphdr * iph;
	struct tcphdr * th;
	struct udphdr * uh;
	int offset;
	
	// printf("Caught Packet of Length %d. \n", phdr->len);
	// printf("Recieved at %s\n", ctime((const time_t *)&phdr->ts.tv_sec));
	// printf("Ethernet Header Length %d\n", ETHER_HDR_LEN);
	
	ehdr = (struct ether_header *) packet;
	int eth_type = ntohs(ehdr->ether_type);
	/* Printing Ethernet Header */
	printf("\nETHER_TYPE: %d\n", eth_type);
	printf("SOURCE ADDRESS: %2x:%2x:%2x:%2x:%2x:%2x\n", ehdr->ether_shost[0]&0xFF, ehdr->ether_shost[1]&0xFF, ehdr->ether_shost[2]&0xFF, ehdr->ether_shost[3]&0xFF, ehdr->ether_shost[4]&0xFF, ehdr->ether_shost[5]&0xFF);
	printf("DESTINATION ADDRESS: %2x:%2x:%2x:%2x:%2x:%2x\n", ehdr->ether_dhost[0]&0xFF, ehdr->ether_dhost[1]&0xFF, ehdr->ether_dhost[2]&0xFF, ehdr->ether_dhost[3]&0xFF, ehdr->ether_dhost[4]&0xFF, ehdr->ether_dhost[5]&0xFF);
	// We want only IP network packets
	if(eth_type == ETHERTYPE_IP) {
		iph = (struct iphdr *)(packet+ETHER_HDR_LEN);
		offset = iph->ihl*4+ETHER_HDR_LEN;
		print_ip_header(iph);
		// We want only TCP Packets
		if(iph->protocol == IPPROTO_TCP) {
			th = (struct tcphdr *)(packet+offset);
			print_tcp_header(th);
			#ifdef APP_DATA
				printf("%s\n", packet+offset+4*th->doff);
			#endif
		} else if(iph->protocol == IPPROTO_UDP) {
			uh = (struct udphdr *)(packet+offset);
			print_udp_header(uh);
			#ifdef APP_DATA
				printf("%s\n", packet+offset+4*th->doff);
			#endif
		}
	}
}

int main(int argc, char * argv[]) {
	char errbuf[PCAP_ERRBUF_SIZE];
	char * dev;
	pcap_t * device;
	
	if (argc < 2) {
		printf("Usage: ./packet <no_of_packet>\n");
		exit(1);
	}
	const int packet_cnt = atoi(argv[1]);
	dev = pcap_lookupdev(errbuf);
	if(dev == NULL) {
		printf("No Network device found !! %s\n", errbuf);
		exit(1);
	}
	printf("Device Found %s\n", dev);
	device = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf); // timeout 0 milli seconds i,e don't wait
	if(device == NULL) {
		printf("%s\n", errbuf);
		exit(1);
	}
	pcap_loop(device, packet_cnt, callback, NULL);
	
	printf("Packet recieving done !! Exiting !!\n");
	return 0;
}

