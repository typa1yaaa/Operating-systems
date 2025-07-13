#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handle_signal(int sig){
  printf("son3 (pid = %d) получил SIGTERT и обработал его\n", getpid());
}

int main(){
  signal(SIGTERM, handle_signal);
  printf("son3 (pid = %d) ожидает сигнал \n", getpid());
  while(1){
    sleep(1);
  }
  return 0;
}
