#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// обработчик сигнала
void handler(int sig, siginfo_t *info, void *uncontext){
  printf("получен сигнал %d (SIGRTMIN %d)\n", sig, sig - SIGRTMIN);
}

int main(){
  struct sigaction act;
  sigset_t mask;
  // задаем обработчик сигнала
  act.sa_sigaction = handler;
  act.sa_flags = SA_SIGINFO;
  // регистрируем обработчик для каждого сигнала, который вызовем далее
  for (int i=0; i<16; i++){
    sigaction(SIGRTMIN + i, &act, NULL);
  }
  
  sigemptyset(&mask);
  // маскируем сигналы 
  for (int i=0; i<16; i++){
    sigaddset(&mask, SIGRTMIN + i);
  }
  //блокируем сигналы 
  sigprocmask(SIG_BLOCK, &mask, NULL);
  
  // отправим сигналы в ранд. порядке
  kill(getpid(), SIGRTMIN + 3);
  kill(getpid(), SIGRTMIN + 14);
  kill(getpid(), SIGRTMIN + 12);
  kill(getpid(), SIGRTMIN + 0);
  kill(getpid(), SIGRTMIN + 5);
  kill(getpid(), SIGRTMIN + 10);
  kill(getpid(), SIGRTMIN + 2);
  kill(getpid(), SIGRTMIN + 7);
  
  // разблокируем сигналы
  sigprocmask(SIG_UNBLOCK, &mask, NULL);
  
  sleep(1);
  return 0;
}
