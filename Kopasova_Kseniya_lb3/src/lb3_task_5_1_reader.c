#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SIZE 256

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    char *shared_mem = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    sem_t *sem_empty = sem_open(SEM_EMPTY, 0);
    sem_t *sem_full = sem_open(SEM_FULL, 0);

    char buffer[SIZE];
    while (1) {
        sem_wait(sem_full);   // ждём, пока данные появятся
        strcpy(buffer, shared_mem);  // читаем
        sem_post(sem_empty);  // сообщаем, что память пуста

        if (strncmp(buffer, "exit", 4) == 0)
            break;

        printf("прочитано сообщение: %s", buffer);
    }

    munmap(shared_mem, SIZE);
    close(shm_fd);
    sem_close(sem_empty);
    sem_close(sem_full);
    return 0;
}

