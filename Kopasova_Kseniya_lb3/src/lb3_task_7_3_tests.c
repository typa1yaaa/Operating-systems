#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h> 

#define PORT 12346
#define BUF_SIZE 1024

// функция для клиента UDP
void *udp_client(void *arg) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    // создание UDP-сокета
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("ошибка socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    buffer[0] = 'A';
    buffer[1] = '\0';

    // отправка сообщения серверу
    sendto(sock, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));
    shutdown(sock, SHUT_WR);
    
    printf("клиент завершает работу\n");
    close(sock);
    return NULL;
}

// функция для клиента TCP
void *tcp_client(void *arg) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    // создание TCP-сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("ошибка socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // подключение к серверу
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("ошибка connect");
        exit(1);
    }

    buffer[0] = 'A';
    buffer[1] = '\0';

    // отправка сообщения серверу
    send(sock, buffer, strlen(buffer), 0);
    
    shutdown(sock, SHUT_WR);
    printf("клиент завершает работу\n");
    close(sock);
    return NULL;
}

void run_test(char *protocol, int num_clients) {
    pthread_t threads[num_clients];
    int i;
    struct timeval start, end;
    long seconds, useconds;
    double duration;

    printf("запуск теста для %d клиентов с использованием %s\n", num_clients, protocol);

    gettimeofday(&start, NULL); 

    // создание клиентов
    for (i = 0; i < num_clients; i++) {
        if (strcmp(protocol, "UDP") == 0) {
            pthread_create(&threads[i], NULL, udp_client, NULL);
        } else if (strcmp(protocol, "TCP") == 0) {
            pthread_create(&threads[i], NULL, tcp_client, NULL);
        }
    }

    // ожидание завершения всех потоков
    for (i = 0; i < num_clients; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);  

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    duration = seconds + useconds / 1000000.0;

    printf("тест завершён за %.4f секунд\n", duration);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("использование: %s <протокол> <количество клиентов>\n", argv[0]);
        return 1;
    }

    char *protocol = argv[1];
    int num_clients = atoi(argv[2]);

    if (num_clients != 10 && num_clients != 100 && num_clients != 1000) {
        printf("количество клиентов должно быть 10, 100 или 1000.\n");
        return 1;
    }

    run_test(protocol, num_clients);

    return 0;
}

