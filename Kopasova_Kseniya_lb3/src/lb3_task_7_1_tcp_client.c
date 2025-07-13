#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_socket"

int main() {
    int sock;
    struct sockaddr_un serv_addr;
    char buffer[1024] = {0};

    // создание сокета UNIX
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("ошибка socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);

    // подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("ошибка connect");
        exit(1);
    }

    // отправка сообщения серверу
    char *msg = "привет, сервер";
    send(sock, msg, strlen(msg), 0);

    // чтение ответа от сервера
    read(sock, buffer, 1024);
    printf("ответ сервера: %s\n", buffer);

    // закрытие сокета
    close(sock);

    return 0;
}

