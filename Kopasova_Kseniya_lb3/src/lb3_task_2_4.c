#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

// переменная для счета количества сигналов
int count = 0;

//функция для потока
void* thread_func(void* arg){
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  int sig;
  while (1){
    printf("поток ожидает SIGINT\n");
    
    sigwait(&set, &sig);
    count++;
    printf("поток получил SIGINT %d раз\n", count);
    if (count >= 3){
      printf("обработчик восстановлен\n");
      // разблокируем sigint для главного потока
      pthread_sigmask(SIG_UNBLOCK, &set, NULL);
    }
  }
  
  printf("поток завершил работу");
  return NULL;
}


int main(){
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  pthread_sigmask(SIG_BLOCK, &set, NULL);
  
  pthread_t t;
  pthread_create(&t, NULL, thread_func, NULL);
  
  // ожидание сигнала 
  while (1){
    printf("программа работает\n");
    sleep(1);
  }
  
  return 0;
}
