#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[] = "Lag Test Packet";
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27015); // Schimbă portul cu cel al serverului tău
    server_addr.sin_addr.s_addr = inet_addr("135.125.166.224"); // Schimbă cu IP-ul serverului tău
    
    while(1) {
        sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        usleep(1000); // Trimite pachet la fiecare 1ms
    }
    
    close(sockfd);
    return 0;
}
