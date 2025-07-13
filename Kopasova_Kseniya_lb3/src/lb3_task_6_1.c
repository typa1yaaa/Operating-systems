#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
  int fd[2];
  pid_t pid;
  char buffer[128];
  
  if (pipe(fd) == -1){
    perror("pipe");
    exit(1);
  }
  
  pid = fork();
  if(pid < 0){
    perror("fork");
    exit(1);
  }else if(pid == 0){
    close(fd[1]);
    read(fd[0], buffer, sizeof(buffer));
    printf("дочерний процесс получил %s", buffer);
    close(fd[0]);
  }else{
    close(fd[0]);
    char msg[] = "привет мир!\n";
    write(fd[1], msg, strlen(msg) + 1);
    close(fd[1]);
  }
  return 0;
}
