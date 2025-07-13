#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

// обработчик для нового sigint
void sigint_handler(int sig){
  printf("перехвачен переназначенный sigint\n");
}

// обработчик для нового sigquit
void sigquit_handler(int sig){
  printf("перехвачен переназначенный sigtstp\n");
  exit(0);
}

int main(){
  signal(SIGINT, sigint_handler);
  signal(SIGQUIT, sigquit_handler);
  
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  
  t.c_cc[VINTR] = 4; // 4 = ascii (ctrl+d)
  t.c_cc[VSUSP] = 24; // 24 = ascii (ctrl+x)
  
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
  printf("ctrl c (sigint) -> ctrl d\n");
  printf("ctrl z (sigquit) -> ctrl x\n");
  
  while(1){
    pause();
  }
  
  return 0;
}

