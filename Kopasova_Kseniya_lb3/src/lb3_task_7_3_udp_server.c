#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12346
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    // cоздание UDP-сокета
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("ошибка socket");
        exit(EXIT_FAILURE);
    }

    // настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // привязка сокета к адресу и порту
    if (bind(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP сервер ожидает подключение\n");

    while (1) {
        int bytes_received = recvfrom(sock, buffer, BUF_SIZE - 1, 0,
                                      (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0) {
            perror("ошибка recvfrom");
            continue;
        }

        buffer[bytes_received] = '\0';
        printf("получено сообщение: %s\n", buffer);

        // отправляем подтверждение
        snprintf(buffer, BUF_SIZE, "привет, клиент!");
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, addr_len);
    }

    close(sock);
    return 0;
}

