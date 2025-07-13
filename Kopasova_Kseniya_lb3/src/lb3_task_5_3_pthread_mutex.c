#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 256

struct shared_data{
    pthread_mutex_t mutex;
    int data_ready;
    char buffer[SIZE];
};

struct shared_data data;

void* reader_thread(void* arg) {
    while (1) {
      pthread_mutex_lock(&data.mutex);
      if (data.data_ready) {
        if (strcmp(data.buffer, "exit\n") == 0) {
          pthread_mutex_unlock(&data.mutex);
          break;
        }
        printf("прочитано сообщение: %s", data.buffer);
        data.data_ready = 0;
      }
      pthread_mutex_unlock(&data.mutex);
      usleep(200000);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&data.mutex, NULL);
    data.data_ready = 0;

    pthread_t reader;
    pthread_create(&reader, NULL, reader_thread, NULL);

    char input[SIZE];
    while (1) {
      printf("введите сообщение ('exit' для выхода): ");
      fflush(stdout);
      fgets(input, SIZE, stdin);

      pthread_mutex_lock(&data.mutex);
     
      strcpy(data.buffer, input);
      data.data_ready = 1;
      pthread_mutex_unlock(&data.mutex);
      if (strcmp(input, "exit\n") == 0)
          break;
      sleep(1);
    }

    pthread_join(reader, NULL);
    pthread_mutex_destroy(&data.mutex);
    return 0;
}


