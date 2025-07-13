#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

//функция для потока
void* thread_func(void* arg){
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  
  printf("поток ожидает SIGINT\n");
  int sig;
  sigwait(&set, &sig);  
  printf("поток получил SIGINT\n");

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
