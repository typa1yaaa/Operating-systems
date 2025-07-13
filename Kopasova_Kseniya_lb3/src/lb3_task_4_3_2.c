#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define SIG1 (SIGRTMIN+4)
#define SIG2 (SIGRTMIN+14)
#define SIG3 (SIGRTMIN+7)

int sig2_count = 0;
int sig3_count = 0;
// обработчик для сигнала sig1
void handle_sig1(int signum){
  printf("sig1 получен\n");
  sleep(15); //для проверки маскировки сигнала sig2
  
  printf("sig1 завершает работу\n");

}

// обработчик для сигнала sig2
void handle_sig2(int signum){
  sig2_count++;
  printf("sig2 получен %d раз\n", sig2_count);
}

// обработчик для сигнала sig3
void handle_sig3(int signum){
  sig3_count++;
  printf("sig3 получен %d раз\n", sig3_count);
}

int main(){
  struct sigaction act_usr1, act_usr2, act_usr3;
  
  // очистим структуру
  memset(&act_usr1, 0, sizeof(act_usr1));
  memset(&act_usr2, 0, sizeof(act_usr2));
  memset(&act_usr3, 0, sizeof(act_usr3));
  
  // указываем обработчики
  act_usr1.sa_handler = handle_sig1;
  act_usr2.sa_handler = handle_sig2;
  act_usr3.sa_handler = handle_sig3;
  
  // маскируем при обработке sig1 и блокируем sig2 и sig3 во время отработки обработчика sig1
  sigemptyset(&act_usr1.sa_mask);
  sigaddset(&act_usr1.sa_mask, SIG2);
  sigaddset(&act_usr1.sa_mask, SIG3);
  
  // устанавливаем обработчики
  sigaction(SIG1, &act_usr1, NULL);
  sigaction(SIG2, &act_usr2, NULL);
  sigaction(SIG3, &act_usr3, NULL);
  
  while (1){
    printf("программа работает, pid = %d\n", getpid());
    sleep(10);
  }
  
  return 0;
}
