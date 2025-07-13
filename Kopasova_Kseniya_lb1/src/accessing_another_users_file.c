#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	if (argc != 2){
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	if (!file){
		perror("Ошибка открытия файла");
		return 1;
	}

	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), file)){
		printf("%s", buffer);
	}
	fclose(file);
	return 0;
}

