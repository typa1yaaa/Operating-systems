#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
  pid_t pid1, pid2, pid3;
  
  if ((pid1 = fork()) == 0){
    sleep(8);
    printf("son1 завершен \n");
    exit(1);
  }
  
  if ((pid2 = fork()) == 0){
    sleep(12);
    printf("son2 завершен \n");
    exit(2);
  }
  
  if ((pid3 = fork()) == 0){
    sleep(16);
    printf("son3 завершен \n");
    exit(3);
  }
  
  printf("ожидание завершения процесса son1 (pid = %d)\n", pid1);
  int status;
  waitpid(pid1, &status, 0);
  printf("son1 завершился с кодом %d\n", WEXITSTATUS(status));
  
  system("ps -l > test_3_2.txt");
  printf("ожидание завершения остальных процессов\n");
  while (wait(NULL) > 0);
  
  printf("все процессы завершены\n");
  return 0;
}

