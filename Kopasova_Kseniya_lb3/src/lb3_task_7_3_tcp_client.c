#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12346
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = "привет, сервер!";

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("ошибка socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // сначала отправляем сообщение
    if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
        perror("ошибка send");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // получаем ответ от сервера
    ssize_t len = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (len < 0) {
        perror("ошибка recv");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    buffer[len] = '\0';
    printf("получено от сервера: %s\n", buffer);

    close(client_socket);
    return 0;
}

