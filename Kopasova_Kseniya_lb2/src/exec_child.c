#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]){
        printf("дочерний процесс начался\n");

        printf("аргументы:\n");
        for (int i=0; argv[i] != NULL; i++){
                printf("argv[%d] = %s\n", i, argv[i]);
        }
        printf("\nпеременные окружения:\n");
        for(int i=0; envp[i] != NULL; i++){
                printf("%s\n", envp[i]);
        }

        printf("дочерний процесс завершился\n");
        return 0;
}
