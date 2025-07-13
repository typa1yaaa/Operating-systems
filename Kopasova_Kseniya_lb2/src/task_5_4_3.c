#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int priorities[3] = {15, 10, 5};

int create_processes(int policy){
  
  for (int i = 0; i < 3; i++){
    pid_t pid = fork();
    if (pid == -1){
      perror("ошибка fork");
      return 1;
      
    }else if (pid == 0){
      struct sched_param param;
      param.sched_priority = priorities[i];
  
      if (sched_setscheduler(0, policy, &param) == -1){
        perror("ошибка установки policy");
        return 1;
      }
      
       struct timespec ts_quantum;
      if (sched_rr_get_interval(0, &ts_quantum) == -1){
        perror("ошибка sched_rr_get_interval");
        return 1;
      }
      
      int p = sched_getscheduler(0);
      for(int i=0; i < 5; i++){
        printf("дочерний процесс pid = %d, приоритет = %d, политика = %d, квант = %ld мс\n", getpid(), param.sched_priority, p, ts_quantum.tv_sec * 1000 + ts_quantum.tv_nsec / 1000000);
        
        int counter = 0;
        for (int i = 0; i < 50; i++){
          counter++;
        }
      }
      
      return 0;
    }
  }
  
  for (int i = 0; i < 3; i++){
    wait(NULL);
  }
}


int main(){
  create_processes(SCHED_RR);
  return 0;
}
