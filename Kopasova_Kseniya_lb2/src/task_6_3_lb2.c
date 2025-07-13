#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void handler(int sig){
  printf("родитель: сигнал получен\n");
}

int main(){
  signal(SIGUSR1, handler);
  
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR2);
  sigprocmask(SIG_BLOCK, &mask, NULL);
  
  pid_t pid = fork();
  if (pid == 0){
    printf("дочерний процесc fork(): проверка обработчика SIGUSR1\n");
    raise(SIGUSR1);
    
    printf("дочерний процесс fork(): проверка блокировки SIGUSR2\n");
    sigprocmask(SIG_SETMASK, NULL, &mask);
    if (sigismember(&mask, SIGUSR2)){
      printf("SIGUSR2 заблокирован\n");
    }
    
    execl("./task_6_3_lb2_child", "./task_6_3_lb2_child", NULL);
    perror("exec");
    exit(1);
  }
  
  wait(NULL);
  return 0;
}
