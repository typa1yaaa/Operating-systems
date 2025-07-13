#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SIZE 256

char buffer[SIZE];
int semid_write, semid_read;

void sem_op(int semid, int op){
  struct sembuf sb = {0, op, 0};
  semop(semid, &sb, 1);
}

void* reader_thread(void* arg) {
    while (1) {
        sem_op(semid_read, -1);
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }
        printf("прочитано сообщение: %s", buffer);
        sem_op(semid_write, 1);
    }
    return NULL;
}

int main() {
    key_t key_write = ftok(".", 'W');
    key_t key_read = ftok(".", 'R');

    semid_write = semget(key_write, 1, IPC_CREAT | 0666);
    semid_read = semget(key_read, 1, IPC_CREAT | 0666);
    
    semctl(semid_write, 0, SETVAL, 1);
    semctl(semid_read, 0, SETVAL, 0);
    
    pthread_t reader;
    pthread_create(&reader, NULL, reader_thread, NULL);

    while (1) {
      sem_op(semid_write, -1);
      
      printf("введите сообщение ('exit' для выхода): ");
      fflush(stdout);
      fgets(buffer, SIZE, stdin);

      sem_op(semid_read, 1);
     
      if (strcmp(buffer, "exit\n") == 0)
          break;
      
    }

    pthread_join(reader, NULL);
    semctl(semid_write, 0, IPC_RMID);
    semctl(semid_read, 0, IPC_RMID);
    return 0;
}


