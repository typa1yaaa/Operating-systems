#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
Программа для задания 2.1. 
Происходит псевдораспараллеливание вычислений 
посредством порождения процесса потомка.
*/ 

void print_process_info(pid_t pid) {
    char path[256];
    FILE *file;
    char comm[256];

    printf("uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);
    file = fopen(path, "r");
    if (file) {
        fgets(comm, sizeof(comm), file);
        printf("comm = %s", comm);
        fclose(file);
    }
    
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    file = fopen(path, "r");
    if (file) {
        char name[256];
        while (fgets(name, sizeof(name), file)) {
            if (strncmp(name, "Name:", 5) == 0) {
                printf("name = %s", name + 6); // Имя процесса
                break;
            }
        }
        fclose(file);
    }
}

int main() {
    pid_t pid = fork(); 

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {  
        printf("дочерний процесс\n");
        print_process_info(getpid());
        
        int res = 0;
	for (int i=0; i<150; i++){
		res++;
	}

        printf("результат вычислений в дочернем процессе = %d\n", res);

        exit(0);
    } else {  
        printf("родительский процесс\n");
        print_process_info(getpid());  
        
        int res = 0;
	for (int i=0; i<150; i++){
		res++;
	}
        printf("результат вычислений в родительском процессе = %d\n", res);
        
        wait(NULL);
        printf("программа завершается\n");
        exit(0);
    }

    return 0;
}
