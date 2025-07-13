#include <sched.h>
#include <stdio.h>

int main(){
  struct sched_param param;
  sched_getparam(0, &param);
  printf("дочерний процесс, политика = %d, приоритет = %d\n", sched_getscheduler(0), param.sched_priority);
  return 0;
}
