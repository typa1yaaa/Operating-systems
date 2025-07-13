#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define FIFO_NAME "myfifo"
#define MAX_LEN 256
#define MSG_COUNT 3

int main() {
  mkfifo(FIFO_NAME, 0666);

  pid_t pid = fork();

  if (pid == 0) {
    FILE *fifo_r = fopen(FIFO_NAME, "r");
    if (!fifo_r) {
      perror("fopen read");
      exit(1);
    }

    char buffer[MAX_LEN];
    for (int i = 0; i < MSG_COUNT; ++i) {
      if (fgets(buffer, MAX_LEN, fifo_r)) {
        printf("получено сообщение: %s", buffer);
      } else {
        printf("ошибка чтения\n");
        break;
      }
    }

    fclose(fifo_r);
    unlink(FIFO_NAME);

  } else {
    FILE *fifo_w = fopen(FIFO_NAME, "w");
    if (!fifo_w) {
        perror("fopen write");
        exit(1);
    }

    char msg[MAX_LEN];
    for (int i = 0; i < MSG_COUNT; ++i) {
      sleep(0.5);
      printf("введите сообщение %d: ", i + 1);
      
      if (fgets(msg, MAX_LEN, stdin)) {
        fputs(msg, fifo_w);
        fflush(fifo_w);
      }
    }

    fclose(fifo_w);
    wait(NULL);
  }

  return 0;
}

