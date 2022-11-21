/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include "messenger.h"

#define READ_END 0
#define WRITE_END 1

// global vars
extern pid_t pid;
extern int fd[2];


int receive_message(){
    //child process
    // will read from the pipe every time the parent process writes to it
    close(fd[WRITE_END]);

    // read string from the pipe
    int n;
    char *message;

    // read messages until parent process there is no more messages
    while (read(fd[READ_END], &n, sizeof(int)) > 0) {
        message = malloc(sizeof(char) * n);
        read(fd[READ_END], &message, sizeof(char) * n);
        printf("receiver.c: Message by child received: %s \n", message);
        free(message);
    }

    close(fd[READ_END]);
    exit(0);
}