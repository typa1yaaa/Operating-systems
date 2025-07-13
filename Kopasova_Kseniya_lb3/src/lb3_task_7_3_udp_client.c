#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12346
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    // создание UDP-сокета
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("ошибка socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // отправляем сообщение
    snprintf(buffer, BUF_SIZE, "привет, сервер!");
    sendto(sock, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    // получаем ответ
    socklen_t addr_len = sizeof(server_addr);
    int bytes_received = recvfrom(sock, buffer, BUF_SIZE - 1, 0,
                                  (struct sockaddr *)&server_addr, &addr_len);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("ответ от сервера: %s\n", buffer);
    }

    close(sock);
    return 0;
}

