#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// обработчик 
void sigint_handler(int signo){
  printf("сигнал SIGINT (CTRL C) принят\n");
  signal(SIGINT, SIG_DFL);
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
