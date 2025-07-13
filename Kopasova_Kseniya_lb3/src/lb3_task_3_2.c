#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int usr2_count = 0;

// обработчик для сигнала sigusr1
void handle_sigusr1(int signum){
  printf("sigusr1 получен\n");
  sleep(5); //для проверки маскировки сигнала sigusr2
  
  printf("sigusr1 завершает работу\n");

}

// обработчик для сигнала sigusr2
void handle_sigusr2(int signum){
  usr2_count++;
  printf("sigusr2 получен %d раз\n", usr2_count);
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
  sigemptyset(&act_usr2.sa_mask);
  sigaddset(&act_usr1.sa_mask, SIGUSR2);
  
  // устанавливаем обработчики
  sigaction(SIGUSR1, &act_usr1, NULL);
  sigaction(SIGUSR2, &act_usr2, NULL);
  
  while (1){
    printf("программа работает, pid = %d\n", getpid());
    sleep(5);
  }
  
  return 0;
}
