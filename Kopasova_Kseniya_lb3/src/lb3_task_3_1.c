#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int usr1_count = 0;
struct sigaction old_usr1;

// обработчик для сигнала sigusr1
void handle_sigusr1(int signum){
  usr1_count++;
  printf("sigusr1 получен %d раз\n", usr1_count);
  sleep(5); //для проверки маскировки сигнала sigusr2
  if (usr1_count >=3){
    printf("достигнут лимит получения sigusr1, восстанавливаем старый обработчик\n");
    sigaction(SIGUSR1, &old_usr1, NULL);
  }
}

// обработчик для сигнала sigusr2
void handle_sigusr2(int signum){
  printf("sigusr2 получен\n");
}

int main(){
  struct sigaction act_usr1, act_usr2;
  
  // очистим структуру
  memset(&act_usr1, 0, sizeof(act_usr1));
  memset(&act_usr2, 0, sizeof(act_usr2));
  
  // указываем обработчики
  act_usr1.sa_handler = handle_sigusr1;
  act_usr2.sa_handler = handle_sigusr2;
  
  // маскируем при обработке sigusr1 и блокируем sigusr2 во время отработки обработчика sigusr1
  sigemptyset(&act_usr1.sa_mask);
  sigaddset(&act_usr1.sa_mask, SIGUSR2);
  
  // устанавливаем обработчики
  sigaction(SIGUSR1, &act_usr1, &old_usr1);
  sigaction(SIGUSR2, &act_usr2, NULL);
  
  
  while (1){
    printf("программа работает, pid = %d\n", getpid());
    sleep(5);
  }
  
  return 0;
}
