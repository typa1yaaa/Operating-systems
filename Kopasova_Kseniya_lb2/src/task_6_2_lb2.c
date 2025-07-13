#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
  struct sched_param param = { .sched_priority = 15};
  sched_setscheduler(0, SCHED_FIFO, &param);
  
  pid_t pid = fork();
  if (pid == 0){
    printf("дочерний процесс, политика = %d, приоритет = %d\n", sched_getscheduler(0), param.sched_priority);
    execl("./task_6_2_lb2_child", "./task_6_2_lb2_child", NULL);
  }
  wait(NULL);
  return 0;
}
