#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct msgbuf{
  long mtype;
  char mtext[100];
};

int main(){
  key_t key = ftok("mailbox", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT); // создадим очередь
  
  
  struct msgbuf message;
  while (1){
    ssize_t r = msgrcv(msgid, &message, sizeof(message.mtext), 1, IPC_NOWAIT);
    if (r == -1){
      if (errno == ENOMSG){
        printf("нет больше сообщений\n");
        break;
      }else{
        perror("ошибка msgrcv");
        exit(1);
      } 
    }
    printf("получено сообщение: %s", message.mtext);
  }
  
  msgctl(msgid, IPC_RMID, NULL);
  return 0;
}
