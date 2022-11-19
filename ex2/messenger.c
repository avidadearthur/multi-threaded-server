/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include "messenger.h"

#define READ_END 0
#define WRITE_END 1

// global vars
extern pid_t pid;
extern int fd[2];

int send_message(char message[]){
    int status;

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
        return 2;
    }
    if (pid > 0){ // parent process
        close(fd[READ_END]);

        printf("Parent says %s\n", message);

        int n = strlen(message) + 1;
        if (write(fd[WRITE_END], &n, sizeof(int)) < 0) {
            return 3;
        }
        printf("Sent n = %d\n", n);

        if (write(fd[WRITE_END], &message, sizeof(char) * n) < 0) {
            return 4;
        }

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

    return 0;
}