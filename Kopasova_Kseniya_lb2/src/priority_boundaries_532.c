#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(){
  printf("SCHED_OTHER: ");
  printf("мин приоритет = %d, макс приоритет = %d\n", sched_get_priority_min(SCHED_OTHER), sched_get_priority_max(SCHED_OTHER));
  
  printf("SCHED_FIFO: ");
  printf("мин приоритет = %d, макс приоритет = %d\n", sched_get_priority_min(SCHED_FIFO), sched_get_priority_max(SCHED_FIFO));
  
  printf("SCHED_RR: ");
  printf("мин приоритет = %d, макс приоритет = %d\n", sched_get_priority_min(SCHED_RR), sched_get_priority_max(SCHED_RR));

  return 0;
}
