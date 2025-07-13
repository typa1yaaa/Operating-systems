#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	printf("дочерний процесс: uid = %d, pid = %d, ppid = %d\n", getuid(), getpid(), getppid());
	system("ps -l > son_processes.txt");
	sleep(10);
	printf("дочерний процесс завершил работу\n");
	return 0;
}
