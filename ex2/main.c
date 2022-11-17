#include <stdio.h>
#include<unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void){
    int status;
    char wmsg[SIZE] = "Greetings";
    char rmsg[SIZE];
    pid_t pid;
    int fd[2];
    // create the pipe
    if (pipe(fd) == -1){
        printf("Pipe failed\n");
        return 1;
    }
    // fork the child
    pid = fork();
    // the parent process will have the pid of the child but the child process
    // will not  be aware of that pid, so it only enters the else clause

    if (pid < 0){ // fork error
        printf("fork failed\n");
        return 1;
    }
    if (pid > 0){ // parent process
        close(fd[READ_END]);

        write(fd[WRITE_END], wmsg, strlen(wmsg)+1);

        close(fd[WRITE_END]);

        //Parent waits process pid (child)
        waitpid(pid, &status, 0);
        //Option is 0 since I check it later

        if (WIFSIGNALED(status)){
            printf("Error\n");
        }
        else if (WEXITSTATUS(status)){
            printf("Exited Normally\n");
        }
        //To Here and see the difference
        printf("Parent process id: %d\n", getpid());
        printf("pid seen from Parent process: %d\n", pid);
    }
    else{ // child process
        close(fd[WRITE_END]);

        read(fd[READ_END], rmsg, SIZE);

        // set to uppercase
        char new_rmsg[SIZE];
        int i = 0;

        // Loop
        while (rmsg[i]) {
            new_rmsg[i] = toupper(rmsg[i]);
            i++;
        }

        printf("Child reads %s \n", new_rmsg);

        close(fd[READ_END]);

        // pid will always be 0 in te child process
        printf("Child process id: %d\n", getpid());
        printf("pid seen from Child process: %d\n", pid);
        exit(EXIT_SUCCESS);
    }
    return 0;
}