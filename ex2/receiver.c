/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include<unistd.h>
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


int receive_message(){
    if (pid <= 0){ // child process
        close(fd[WRITE_END]);

        int n;
        char *str;

        if (read(fd[READ_END], &n, sizeof(int)) < 0) {
            return 5;
        }
        printf("Received n = %d\n", n);
        if (read(fd[READ_END], &str, sizeof(int) * n) < 0) {
            return 6;
        }
        printf("Received string %s\n", str);

        // set to uppercase
        char new_rmsg[sizeof(int) * n];
        int i = 0;

        // Loop
        while (str[i]) {
            new_rmsg[i] = toupper(str[i]);
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