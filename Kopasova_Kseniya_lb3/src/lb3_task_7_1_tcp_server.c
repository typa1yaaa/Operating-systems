#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/my_socket"

int main() {
    int server_fd, new_socket;
    struct sockaddr_un address;
    char buffer[1024] = {0};

    // создание сокета UNIX
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("ошибка socket");
        exit(1);
    }

    // удаление старого файла сокета, если он существует
    unlink(SOCKET_PATH);

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);

    // привязка сокета
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("ошибка bind");
        exit(1);
    }

    // ожидание соединений
    if (listen(server_fd, 3) == -1) {
        perror("ошибка listen");
        exit(1);
    }

    printf("сервер ожидает подключение\n");

    // принятие соединения
    new_socket = accept(server_fd, NULL, NULL);
    if (new_socket == -1) {
        perror("ошибка accept");
        exit(1);
    }

    // чтение данных от клиента
    read(new_socket, buffer, 1024);
    printf("получено сообщение: %s\n", buffer);

    // ответ клиенту
    send(new_socket, "сегодня хороший день", strlen("сегодня хороший день"), 0);

    // закрытие соединений
    close(new_socket);
    close(server_fd);

    return 0;
}

