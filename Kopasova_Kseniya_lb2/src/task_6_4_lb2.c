#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

struct child_args {
  int flags;
  int fd;
};

// глобальная переменная
int shared_var = 100;

int child_func(void *arg) {
  struct child_args *args= (struct child_args *)arg;
  
  printf("дочерний pid = %d\n", getpid());

  // проверка флага CLONE_VM(общая память)
  if (args->flags & CLONE_VM) {
      shared_var = 200;
      printf("дочерний изменил shared_var на %d (флаг CLONE_VM)\n", shared_var);
  } else {
      printf("дочерний shared_var = %d (нет флага CLONE_VM)\n", shared_var);
  }

  // проверка CLONE_FILES(общие файловые дескрипторы)
  if (args->flags & CLONE_FILES) {
      printf("дочерний записывает общий файл\n");
      if (args->fd != -1) {
          write(args->fd, "дочерний процесс записал информацию\n", 29);
          fsync(args->fd);
      }
  }

  // проверка CLONE_FS(общая файловая система)
  if (args->flags & CLONE_FS) {
      printf("дочерний меняет рабочую директорию на /tmp\n");
      chdir("/tmp");
  }

  return 0;
}

void test_clone_flags(int flags, const char *description) {
  printf("\nтестируем флаги: %s\n", description);
  shared_var = 100; // сбрасываем переменную

  int fd = open("lb2_6_4.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  write(fd, "родительский процесс записал информацию\n", 33);
  fsync(fd);
  
  struct child_args args = {
    .flags = flags,
    .fd = fd
  };
  
  char stack[4096]; 
  pid_t pid = clone(child_func, stack + 4096, flags | SIGCHLD, &flags);

  sleep(1);

  printf("родительский процесс:\n");
  printf("родительский shared_var = %d\n", shared_var);

  // проверяем содержимое файла
  if (flags & CLONE_FILES) {
      printf("родительский файл после дочернего процесса:\n");
      lseek(fd, 0, SEEK_SET);
      char buf[1024];
      int bytes = read(fd, buf, sizeof(buf));
      if (bytes > 0){
        write(STDOUT_FILENO, buf, bytes);
      }
  } else {
      printf("родительский файл не был общим (нет флага CLONE_FILES)\n");
  }

  // проверяем рабочую директорию
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  printf("родительская текущая директория: %s\n", cwd);

  waitpid(pid, NULL, 0);
  close(fd);
  remove("lb2_6_4.txt"); 
}

int main() {
  // только SIGCHLD (как fork())
  test_clone_flags(SIGCHLD, "только SIGCHLD (аналог fork())");

  // общая память (CLONE_VM)
  test_clone_flags(CLONE_VM | SIGCHLD, "CLONE_VM (общая память)");

  // общие файловые дескрипторы (CLONE_FILES)
  test_clone_flags(CLONE_FILES | SIGCHLD, "CLONE_FILES (общие файловые дескрипторы)");

  // общая файловая система (CLONE_FS)
  test_clone_flags(CLONE_FS | SIGCHLD, "CLONE_FS (общая файловая система)");

  // все флаги вместе
  test_clone_flags(CLONE_VM | CLONE_FILES | CLONE_FS | SIGCHLD, 
                  "CLONE_VM | CLONE_FILES | CLONE_FS");

  return 0;
}
