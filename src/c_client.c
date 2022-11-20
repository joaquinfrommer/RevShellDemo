#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#define PORT 8080

int main(int argc, char *argv[]) {
    int socket_fd, len;
    struct sockaddr_in servaddr;
    char buffer[4096];

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.236");
    servaddr.sin_port = htons(PORT);
    if(connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        read(socket_fd, buffer, sizeof(buffer));
        printf(buffer);
        fflush(stdout);
        memset(buffer, 0, sizeof(buffer));
        len = read(0, buffer, sizeof(buffer));
        if (!strncmp(buffer, "bye", sizeof("bye"))) {
            break;
        }
        send(socket_fd, buffer, len, 0);
        memset(buffer, 0, sizeof(buffer));
    }
    return 0; 
}