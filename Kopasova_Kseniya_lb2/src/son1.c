#include <stdio.h>
#include <unistd.h>

int main(){
  printf("son1 (pid = %d) ожидает сигнал \n", getpid());
  while(1){
    sleep(1);
  }
  return 0;
}
