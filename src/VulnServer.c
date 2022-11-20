#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#define PORT 8080

void handle_client(int client_sock) {
    char *good = "Good email!\n";
    char *bad = "This email is terrible, did you forget everything we learned in class?\n";
    // Memory chunck with a size of 2048 characters 
    char buffer[2048];
    printf("%p", buffer);
    //Read 2048 characters 
    while (1) {
        if(read(client_sock, buffer, 3048) > 0) {
            printf("Recieved data...\n");
            if (!strncmp(buffer, "bye", 3)) {
                printf("Connection over...\n");
                send(client_sock, "Bye\n", strlen("Bye\n"), 0);
                break;
            } else if (rand() % 2 == buffer[0] % 2) { 
                send(client_sock, good, strlen(good), 0);
            } else {
                send(client_sock, bad, strlen(bad), 0);
            }
        }
    }
}

void run_server() {
    int socket_fd, client_sock, client_size, opt = 1;
    struct sockaddr_in server_addr, client_addr;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    client_size = sizeof(client_addr);
    if((client_sock = accept(socket_fd, (struct sockaddr*)&client_addr, &client_size)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected!\n");
    send(client_sock, "hi\n", strlen("hi\n"), 0);
    handle_client(client_sock);
    close(client_sock);
    close(socket_fd);

}

int main(int argc, char *argv[]) {
    printf("Starting Server ...\n");
    fflush(stdout);
    run_server();
    printf("Server Shutting Down ...\n");
    return 0;
}
