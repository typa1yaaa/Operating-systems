#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 256

struct shared_data{
  sem_t sem_write;
  sem_t sem_read;
  char buffer[SIZE];
};

struct shared_data data;

void* reader_thread(void* arg) {
    while (1) {
      sem_wait(&data.sem_read);
      if (strcmp(data.buffer, "exit\n") == 0)
          break;
      
      printf("прочитано сообщение: %s", data.buffer);
      sem_post(&data.sem_write);
    }
    return NULL;
}

int main(){
  sem_init(&data.sem_write, 0, 1);
  sem_init(&data.sem_read, 0, 0);
  
  pthread_t reader;
  pthread_create(&reader, NULL, reader_thread, NULL);

  while (1) {
    sem_wait(&data.sem_write);
    
    printf("введите сообщение ('exit' для выхода): ");
    fflush(stdout);
    fgets(data.buffer, SIZE, stdin);

    sem_post(&data.sem_read);
   
    if (strcmp(data.buffer, "exit\n") == 0)
        break;
    
  }

  pthread_join(reader, NULL);
  sem_destroy(&data.sem_write);
  sem_destroy(&data.sem_read);
  
  return 0;
}



