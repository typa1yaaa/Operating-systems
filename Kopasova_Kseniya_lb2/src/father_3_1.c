#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
  pid_t pid1, pid2, pid3;
  
  if ((pid1 = fork()) == 0){
    execl("./son1", "son1", NULL);
    perror("ошибка запуска son1");
    exit(1);
  }
  
  if ((pid2 = fork()) == 0){
    execl("./son2", "son2", NULL);
    perror("ошибка запуска son2");
    exit(1);
  }
  
  if ((pid3 = fork()) == 0){
    execl("./son3", "son3", NULL);
    perror("ошибка запуска son3");
    exit(1);
  }
  
  sleep(3);
  system("ps -s > test_3_1.txt");
  
  printf("родительский процесс отправляет сигнал SIGTERM процессу son1 (%d)\n", pid1);
  kill(pid1, SIGTERM);
  
    printf("родительский процесс отправляет сигнал SIGTERM процессу son2 (%d)\n", pid2);
  kill(pid2, SIGTERM);
  
    printf("родительский процесс отправляет сигнал SIGTERM процессу son3 (%d)\n", pid3);
  kill(pid3, SIGTERM);
  sleep(3);
  
  system("ps -s >> test_3_1.txt");
  return 0;
}
