#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// переменная для счета количества сигналов
int count = 0;

// обработчик 
void sigint_handler(int signo){
  count++;
  printf("сигнал SIGINT (CTRL C) принят %d раз\n", count);
  if (count >= 3){
    printf("обработчик восстановлен\n");
    signal(SIGINT, SIG_DFL);
  }
}


int main(){
  // устанавливаем обработчик 
  signal(SIGINT, sigint_handler);

  // ожидание сигнала 
  while (1){
    printf("программа работает\n");
    sleep(1);
  }
  
  return 0;
}
