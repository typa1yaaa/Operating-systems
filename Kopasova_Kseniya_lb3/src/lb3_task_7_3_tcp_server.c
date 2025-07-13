#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12346
#define BUFFER_SIZE 1024


int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len;
  char buffer[BUFFER_SIZE];

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("ошибка socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("ошибка bind");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, 10) < 0) {
    perror("ошибка listen");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("TCP сервер ожидает подключение\n");

  while (1) {
    client_addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket < 0) {
      perror("ошибка client_socket");
      continue;
    }

    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
      if (bytes_received < 0) {
        perror("ошибка чтения от клиента");
      } else {
        buffer[bytes_received] = '\0';
        printf("получено сообщение: %s\n", buffer);

        // отправляем ответ
        snprintf(buffer, sizeof(buffer), "привет, клиент!");
        send(client_socket, buffer, strlen(buffer), 0);
      }

      close(client_socket);
  }

  close(server_socket);
  return 0;
}

