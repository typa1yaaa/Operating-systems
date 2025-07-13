#include <unistd.h>
#include <sched.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
  int fd = open("lb2_6_1.txt", O_RDWR | O_CREAT, 0644);
  write(fd, "before exec\n", 12);
  
  pid_t pid = fork();
  if (pid == 0){
    char fd_str[20];
    sprintf(fd_str, "%d", fd);
    execl("./exec_6_1_child_program", "./exec_6_1_child_program", fd_str, NULL);
    perror("ошибка exec");
    return 1;
  }
  
  wait(NULL);
  lseek(fd, 0, SEEK_SET);
  char buf[30];
  read(fd, buf, 30);
  printf("родитель прочитал: %s\n", buf);
  close(fd);
  return 0;
}
