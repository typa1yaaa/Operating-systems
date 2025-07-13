#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void run_scenario_a() {
    printf("а) Процесс-отец ожидает завершения процесса-сына\n");
    pid_t pid = fork();

    if (pid < 0) {
        perror("ошибка fork");
        exit(1);
    }

    if (pid == 0) {
        printf("процесс-сын: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
        sleep(2); 
        printf("процесс-сын завершил работу\n");
        exit(0);
    } else {
        printf("родительский процесс: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
        system("ps -l > scenario_a_processes.txt");
        wait(NULL);
        system("ps -l >> scenario_a_processes.txt");
        printf("родительский процесс завершил работу\n");
    }
}

void run_scenario_b() {
    printf("б) Процесс-отец завершается, не ожидая процесса-сына\n");
    pid_t pid = fork();

    if (pid < 0) {
        perror("ошибка fork");
        exit(1);
    }

    if (pid == 0) {
        printf("процесс-сын: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
        sleep(5);  
        printf("процесс-сын: новый ppid = %d (после завершения отца)\n", getppid());
        printf("процесс-сын завершил работу\n");
        exit(0);
    } else {
        printf("родительский процесс: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
        system("ps -l > scenario_b_processes.txt");
        sleep(1);
        printf("родительский процесс завершил работу\n");
        exit(0); 
    }
}

void run_scenario_c() {
    printf("в) Процесс-сын становится зомби\n");
    pid_t pid = fork();

    if (pid < 0) {
        perror("ошибка fork");
        exit(1);
    }

    if (pid == 0) {
        printf("процесс-сын: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
        sleep(2); 
        printf("процесс-сын завершил работу\n");
        exit(0);
    } else {
        printf("родительский процесс: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
        sleep(1);  
        system("ps -l > scenario_c_processes.txt"); 
        sleep(5); 
        system("ps -l >> scenario_c_processes.txt");  
        printf("родительский процесс завершил работу\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Использование: %s <номер сценария (1, 2 или 3)>\n", argv[0]);
        return 1;
    }

    int scenario = atoi(argv[1]);

    switch (scenario) {
        case 1:
            run_scenario_a();
            break;
        case 2:
            run_scenario_b();
            break;
        case 3:
            run_scenario_c();
            break;
        default:
            printf("Неверный номер сценария. Используйте 1, 2 или 3.\n");
            return 1;
    }

    return 0;
}
