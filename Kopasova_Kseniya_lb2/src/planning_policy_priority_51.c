#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(){
  pid_t pid = getpid();
  int policy = sched_getscheduler(pid);
  
  int priority = getpriority(PRIO_PROCESS, pid);
  
  printf("политика: %d\n", policy);
  printf("приоритет: %d\n", priority);
  return 0;
}
