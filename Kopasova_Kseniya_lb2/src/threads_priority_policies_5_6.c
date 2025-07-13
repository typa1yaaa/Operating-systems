#include <pthread.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>

void set_policy(pthread_t thread, int policy, int priority){
  struct sched_param param = {.sched_priority = priority};
  pthread_setschedparam(thread, policy, &param);
}

void print_info_thread(int id){
  int policy = sched_getscheduler(0);
  struct sched_param param;
  sched_getparam(0, &param);
  printf("поток %d с политикой = %d и приоритетом = %d \n", id, policy, param.sched_priority);
}

void* thread_func(void* arg){
  sleep(1);
  print_info_thread(*(int*) arg);
  for (int i = 0; i < 3; i++){
    printf("поток %d работает\n", *(int*) arg);
    for (int j = 0; j < 50; j++){}
  }
  return NULL;
}

int main(){
  pthread_t thread_1, thread_2, thread_3;
  int id1 = 1, id2 = 2, id3 = 3;
  
  pthread_create(&thread_1, NULL, thread_func, &id1);
  pthread_create(&thread_2, NULL, thread_func, &id2);
  pthread_create(&thread_3, NULL, thread_func, &id3);
  
  set_policy(thread_1,  SCHED_RR, 99);
  set_policy(thread_2, SCHED_RR, 50);
  set_policy(thread_3, SCHED_RR, 1);
  
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);
  pthread_join(thread_3, NULL);
  return 0;
}
