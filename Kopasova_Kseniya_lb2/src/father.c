#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	pid_t pid = fork();
	if (pid < 0){
		perror("ошибка fork");
		return 1;
	}
	
	if (pid == 0){
		execl("./son", "son", NULL);
		return 1;
	}else{
		printf("родительский процесс: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
		//system("ps -l > father_processes.txt");
		//wait(NULL);
		sleep(2);		
		system("ps -l > father_processes.txt");
                sleep(9);
		system("ps -l >> father_processes.txt");
		printf("родительский процесс завершил работу\n");

	}

	return 0;
}
