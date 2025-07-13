#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf{
  long mtype;
  char mtext[100];
};

int main(){
  key_t key = ftok("mailbox", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT); // создадим очередь
  
  
  struct msgbuf message;
  message.mtype = 1;
  printf("введите сообщение: ");
  fgets(message.mtext, sizeof(message.mtext), stdin);
  
  msgsnd(msgid, &message, strlen(message.mtext) + 1, 0);
  printf("сообщение отправлено\n");

  return 0;
}
