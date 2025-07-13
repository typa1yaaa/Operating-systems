#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SIZE 256

struct shared_data{
  pthread_mutex_t mutex;
  int data_ready;
  char buffer[SIZE];
};

int main(){
  struct shared_data* data = mmap(NULL, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&data->mutex, &attr);
  data->data_ready = 0;
  
  pid_t pid = fork();
  if (pid == 0){
    // читатель
    while(1){
      pthread_mutex_lock(&data->mutex);
      if (data->data_ready){
        if (strcmp(data->buffer, "exit\n") == 0){
           pthread_mutex_unlock(&data->mutex);
           break;
        }
        printf("прочитано сообщение: %s", data->buffer);
        //printf("введите сообщение ('exit' для выхода): ");
        fflush(stdout);
        data->data_ready = 0;
      }
      pthread_mutex_unlock(&data->mutex);
      usleep(100000);
    }
  }else{
    // писатель
    char input[SIZE];
    while (1){
      printf("введите сообщение ('exit' для выхода): ");
      fflush(stdout);
      fgets(input, SIZE, stdin);
      pthread_mutex_lock(&data->mutex);
      
      strcpy(data->buffer, input);
      data->data_ready = 1;
      pthread_mutex_unlock(&data->mutex);
      if (strcmp(input, "exit\n") == 0){
         break;
      }
      sleep(1);
    }
    wait(NULL);
  }
  
  pthread_mutex_destroy(&data->mutex);
  pthread_mutexattr_destroy(&attr);
  munmap(data, sizeof(struct shared_data));
  return 0;
}



