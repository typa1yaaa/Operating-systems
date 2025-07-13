#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>


pthread_t t1, t2;

// обработчик 
void sigusr1_handler(int signo){
  if (signo ==SIGUSR1){
    printf("поток 2: сигнал SIGUSR1 принят\n");
    printf("поток 2 завершил работу\n");
    pthread_exit(NULL); 
  }
}

// функция для второго потока
void* thread2_func(void* arg){
  //установка обработчика сигнала
  struct sigaction sa;
  sa.sa_handler = sigusr1_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGUSR1, &sa, NULL);
  
  printf("поток 2: pid = %d, tid = %lu\n", getpid(), syscall(SYS_gettid));
  while(1){
    printf("поток 2 работает \n");
    sleep(1);
  }
  
  return NULL;
}

// функция для первого потока
void* thread1_func(void* arg){
  printf("поток 1: pid = %d, tid = %lu\n", getpid(),  syscall(SYS_gettid));
  
  // создаем поток 2
  if (pthread_create(&t2, NULL, thread2_func, NULL) != 0){
    perror("ошибка pthread_create");
    return NULL;
  }
  
  // задаем интервал, после которого поток 1 отправит потоку 2 SIGUSR1
  int interval = 5;
  printf("поток 1: SIGUSR1 будет отправлен потоку 2 после %d секунд, %ld \n", interval, time(NULL));
  
  // ожидаем interval секунд перед отправкой сигнала
  sleep(interval);
  
  printf("поток 1: SIGUSR1 отправлен потоку 2 в %ld секунд\n", time(NULL));
  pthread_kill(t2, SIGUSR1);
  
  // завершаем второй поток
  printf("поток 1: поток 2 остановлен в %ld секунд\n", time(NULL));

  // ждем завершения второго потока
  pthread_join(t2, NULL);
  
  printf("поток 1 завершил работу\n");
  return NULL;
}

int main(){
  // создаем первый поток
  if (pthread_create(&t1, NULL, thread1_func, NULL) != 0){
    perror("ошибка pthread_create");
    return 1;
  }
  // ждем завершения первого потока
  pthread_join(t1, NULL);
  return 0;
}
