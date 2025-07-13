#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define SIG1 (SIGRTMIN+4)

volatile sig_atomic_t last_seq = 0;

// обработчик для сигнала sig1
void handle_sig1(int signum, siginfo_t *info, void* ucontext){
  int cur_seq = info->si_value.sival_int;
  printf("sig1 %d->%d получен \n", last_seq, cur_seq);
  last_seq = cur_seq;
}

int main(){
  struct sigaction act_usr1;
  union sigval value;
  
  // очистим структуру
  memset(&act_usr1, 0, sizeof(act_usr1));
  
  // указываем обработчики
  act_usr1.sa_sigaction = handle_sig1;
  act_usr1.sa_flags = SA_SIGINFO;
  
  // устанавливаем обработчики
  sigaction(SIG1, &act_usr1, NULL);
  
  // подаем сигналы с порядковыми номерами
  for (int i=1; i<=5; i++){
    value.sival_int = i;
    sigqueue(getpid(), SIG1, value);
    usleep(100000);
  }
  
  sleep(1);
  return 0;
}
