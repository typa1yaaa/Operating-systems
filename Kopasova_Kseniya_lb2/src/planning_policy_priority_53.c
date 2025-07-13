#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void measure_time_quantum(){
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  while (1){
    clock_gettime(CLOCK_MONOTONIC, &end);
    if ((end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec) > 1000000){
      break;
    }
  }
  printf("кванты времени = %ld\n", (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)); 
}


int main(){
  measure_time_quantum();
  
  pid_t pid = fork();
  
  if (pid == -1){
    perror("ошибка fork");
    return 1;
  }
  
  if (pid == 0){
    struct sched_param param;
    param.sched_priority = 0;
    
    
    if (sched_setscheduler(0, SCHED_OTHER, &param) == -1){
      perror("ошибка установки SCHED_OTHER");
      return 1;
    }
    
    int policy = sched_getscheduler(0);
    for(int i=0; i < 5; i++){
      printf("дочерний процесс pid = %d, приоритет = %d, политика = %d\n", getpid(), param.sched_priority, policy);
      if (i % 2 == 0){
        printf("дочерний процесс освобождает процессор\n");
        sched_yield();
      }
      sleep(1);
    }
  }else{
    struct sched_param param;
    param.sched_priority = 0;
    
    if (sched_setscheduler(0, SCHED_RR , &param) == -1){
      perror("ошибка установки SCHED_RR");
      return 1;
    }
    
    
    int policy = sched_getscheduler(0);
    for(int i=0; i < 5; i++){
      printf("родительский процесс pid = %d, приоритет = %d, политика = %d\n", getpid(), param.sched_priority, policy);
      sleep(1);
    }
    
    wait(NULL);
  }
  
  return 0;
}
