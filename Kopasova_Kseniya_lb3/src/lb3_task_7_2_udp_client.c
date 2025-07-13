#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_udp_socket"
#define CLIENT_PATH "/tmp/my_udp_client" 

int main() {
    int sockfd;
    struct sockaddr_un serv_addr, cli_addr;
    char buffer[1024] = {0};

    // создание UDP сокета UNIX
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("ошибка socket");
        exit(1);
    }

    // удаление старого файла клиента, если он существует
    unlink(CLIENT_PATH);

    // привязка клиентского сокета
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sun_family = AF_UNIX;
    strcpy(cli_addr.sun_path, CLIENT_PATH);
    if (bind(sockfd, (struct sockaddr *)&cli_addr, sizeof(cli_addr)) == -1) {
        perror("ошибка bind клиента");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);

    // отправка сообщения серверу
    const char *msg = "привет, сервер";
    if (sendto(sockfd, msg, strlen(msg), 0,
               (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("ошибка sendto");
        exit(1);
    }

    // получение ответа от сервера
    socklen_t serv_len = sizeof(serv_addr);
    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                        (struct sockaddr *)&serv_addr, &serv_len);
    if (n == -1) {
        perror("ошибка recvfrom");
        exit(1);
    }
    printf("ответ сервера: %s\n", buffer);

    // закрытие сокета
    close(sockfd);
    unlink(CLIENT_PATH);

    return 0;
}
