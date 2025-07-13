#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_udp_socket"

int main() {
    int sockfd;
    struct sockaddr_un serv_addr, cli_addr;
    socklen_t cli_len;
    char buffer[1024] = {0};

    // создание UDP сокета UNIX
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("ошибка socket");
        exit(1);
    }

    // удаление старого файла сокета, если он существует
    unlink(SOCKET_PATH);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);

    // привязка сокета
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("ошибка bind");
        exit(1);
    }

    printf("сервер ожидает подключение\n");

    // получение сообщения от клиента
    cli_len = sizeof(cli_addr);
    ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                        (struct sockaddr *)&cli_addr, &cli_len);
    if (n == -1) {
        perror("ошибка recvfrom");
        exit(1);
    }
    printf("получено сообщение: %s\n", buffer);

    // отправка ответа клиенту
    const char *reply = "сегодня хороший день";
    if (sendto(sockfd, reply, strlen(reply), 0,
               (struct sockaddr *)&cli_addr, cli_len) == -1) {
        perror("ошибка sendto");
        exit(1);
    }

    // закрытие сокета
    close(sockfd);
    unlink(SOCKET_PATH);

    return 0;
}
