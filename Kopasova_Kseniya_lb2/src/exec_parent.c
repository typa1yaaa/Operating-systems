#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
        printf("pодительский процесс начался\n");
        char *args[] = {"exec_child", "arg1", "arg2", "arg3", NULL};
        char *env[] = {"MY_VAR=Hello", "TEST_VAR=42", NULL};

        //execl("./exec_child", "exec_child", "arg1", "arg2", "arg3", NULL);
        //execle("./exec_child", "exec_child", "arg1", "arg2", "arg3", NULL, env);
       	//execlp("ls", "ls", "-l", NULL);
        execv("./exec_child", args);
        //execvp("exec_child", args);
        //execve("./exec_child", args, env);
        return 1;
}

