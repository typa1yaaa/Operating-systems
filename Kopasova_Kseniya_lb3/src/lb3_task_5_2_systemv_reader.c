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
  int shmid = shmget(SHM_KEY, SIZE, 0666);
  char* shared_mem = (char *)shmat(shmid, NULL, 0);
  
  int semid = semget(SEM_KEY, 2, 0666);
  
  char buffer[SIZE];
  while (1){
    P.sem_num = 1; semop(semid, &P, 1);
    strcpy(buffer, shared_mem);
    V.sem_num = 0; semop(semid, &V, 1);
    
    if (strncmp(buffer, "exit", 4) == 0)
      break;
    
    printf("прочитано сообщение: %s", buffer);
    
  }
  
  shmdt(shared_mem);
  return 0;
}
