#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define SIZE 256

struct sembuf P = {0, -1, 0}; 
struct sembuf V = {0, 1, 0}; 

int main(){
  int shmid = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666);
  char* shared_mem = (char *)shmat(shmid, NULL, 0);
  
  int semid = semget(SEM_KEY, 2, IPC_CREAT | 0666);
  semctl(semid, 0, SETVAL, 1);
  semctl(semid, 1, SETVAL, 0);
  
  char buffer[SIZE];
  while (1){
      printf("введите сообщение ('exit' для выхода): ");
      fgets(buffer, SIZE, stdin);
      P.sem_num = 0; semop(semid, &P, 1);
      strcpy(shared_mem, buffer);
      V.sem_num = 1; semop(semid, &V, 1);
      
      if (strncmp(buffer, "exit", 4) == 0)
        break;
  }
  
  shmdt(shared_mem);
  return 0;
}
