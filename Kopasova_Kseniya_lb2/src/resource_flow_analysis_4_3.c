#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <string.h>

int shared_value = 0; // глобальная переменная
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *arg){
  int thread_num = *(int *)arg;
  
  int local_var = thread_num * 10; // уникальные данные (стек)
  printf("поток %d: локальная переменная = %d (адрес %p)\n", thread_num, local_var, (void *)&local_var);
  
  pthread_mutex_lock(&mutex); // разделяемые данные (глоб. переменная)
  shared_value += 1;
  printf("поток %d: глобальная переменная = %d\n", thread_num, shared_value);
  pthread_mutex_unlock(&mutex);
  
  int fd = open("lb2_test43.txt", O_WRONLY | O_APPEND); // разделяемые файловые дескрипторы
  if (fd == -1){
    perror("open");
    return NULL;
  }
  dprintf(fd, "поток %d записал данные\n", thread_num);
  close(fd);
  
  char* env_var = getenv("MY_VAR");// разделяемые переменные окружения
  if (env_var){
    printf("поток %d: переменная окружения MY_VAR = %s\n", thread_num, env_var);
  } else{
    printf("поток %d: переменная окружения MY_VAR не найдена\n", thread_num);
  }
  
  printf("поток %d: tid = %ld, pid = %d\n", thread_num, syscall(SYS_gettid), getpid());
  return 0;
}

int main(){
  const int NUM_THREADS = 3;
  pthread_t threads[NUM_THREADS];
  int thread_args[NUM_THREADS];
  
  setenv("MY_VAR", "Hello!", 1);
  
  int fd = open("lb2_test43.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1){
    perror("ошибка open");
    exit(EXIT_FAILURE);
  }
  close(fd);
  
  for (int i=0; i < NUM_THREADS; i++){
    thread_args[i] = i + 1;
    if (pthread_create(&threads[i], NULL, thread_func, &thread_args[i]) != 0){
      perror("ошибка pthread_create");
      exit(EXIT_FAILURE);
    }
  }
  
  for (int i=0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }
  
  system("cat lb2_test43.txt");
  return 0;
}

