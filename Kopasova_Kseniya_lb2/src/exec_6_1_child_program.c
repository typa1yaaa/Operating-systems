#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  int fd = atoi(argv[1]);
  write(fd, "after exec", 11);
  fsync(fd);
  close(fd);
  return 0;
}
