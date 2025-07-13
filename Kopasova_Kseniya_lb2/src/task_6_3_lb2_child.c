#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  printf("дочерний процесc exec(): проверка обработчика SIGUSR1\n");
  if (signal(SIGUSR1, SIG_IGN) == SIG_DFL){
    printf("обработчик SIGUSR1 сброшен в SIG_DFL\n");
  }
  
  printf("дочерний процесс exec(): проверка блокировки SIGUSR2\n");
  sigset_t cur_mask;
  sigprocmask(SIG_SETMASK, NULL, &cur_mask);
  if (sigismember(&cur_mask, SIGUSR2)){
    printf("SIGUSR2 заблокирован\n");
  }
  return 0;
}
