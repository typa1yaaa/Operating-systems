#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

struct msgbuf{
  long mtype;
  char mtext[100];
};

int main(){
  key_t key = ftok("file_6_3", 65);
  int msgid = msgget(key, 0666 | IPC_CREAT);
  
  if (fork() == 0){
    struct msgbuf message;
    msgrcv(msgid, &message, sizeof(message.mtext), 1, 0);
    printf("получено сообщение: %s", message.mtext);
  }else{
    struct msgbuf message;
    message.mtype = 1;
    strcpy(message.mtext, "сегодня хороший день\n");
    msgsnd(msgid, &message, sizeof(message.mtext), 0);
    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
  }
  
  return 0;
}
