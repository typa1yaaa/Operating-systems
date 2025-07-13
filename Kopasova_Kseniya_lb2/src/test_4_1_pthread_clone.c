#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <time.h>
#include <sched.h>
#include <sys/resource.h>

#define STACK_SIZE 1024*1024

void* pthread_func(void* arg){
  int id = *(int*) arg;
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  printf("pthread: поток %d запущен, tid = %ld, pid = %d", id, syscall(SYS_gettid), getpid());
  sleep(50);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  
  printf("pthread: поток %d завершил работу за %.6f сек\n", id, duration);
  return NULL;
}

int clone_func(void* arg){
  int id = *(int*) arg;
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  
  printf("clone: поток %d запущен, tid = %ld, pid = %d", id, syscall(SYS_gettid), getpid());
  sleep(50);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
  
  printf("clone: поток %d завершил работу за %.6f сек\n", id, duration);
  return 0;
}

int main(){
  const int NUM_THREADS = 3;
  pthread_t threads[NUM_THREADS];
  int thread_args[NUM_THREADS];
  
  for (int i=0; i < NUM_THREADS; i++){
    thread_args[i] = i + 1;
    if (pthread_create(&threads[i], NULL, pthread_func, &thread_args[i]) != 0){
      perror("ошибка pthread_create");
      exit(EXIT_FAILURE);
    }
  }
  
  for (int i=0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
    }
  
  for (int i=0; i < NUM_THREADS; i++){
    void *stack = malloc(STACK_SIZE);
    if (!stack){
      perror("ошибка stack");
      exit(EXIT_FAILURE);
    }
    
    void *stack_top = stack + STACK_SIZE;
    stack_top = (void *) (((long)stack_top) & ~0xFFF);
    thread_args[i] = i + 1;
    pid_t pid = clone(clone_func, stack_top, CLONE_VM | CLONE_SIGHAND | CLONE_THREAD | CLONE_FILES, &thread_args[i]);
    
    if (pid == -1){
      perror("ошибка clone");
      free(stack);
      exit(EXIT_FAILURE);
    }
    usleep(10000000);
    free(stack);

  }
  
  printf("главный поток завершен\n");
  return 0;
}

