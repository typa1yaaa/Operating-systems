#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void ignore_signal(int sig){
  printf("son2 (pid = %d) получил SIGTERM, но игнорирует его\n", getpid());
}

int main(){
  signal(SIGTERM, ignore_signal);
  printf("son2 (pid = %d) ожидает сигнал \n", getpid());
  while(1){
    sleep(1);
  }
  return 0;
}
