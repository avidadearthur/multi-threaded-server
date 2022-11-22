/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h>
#include "messenger.h"

#define READ_END 0
#define WRITE_END 1

// global vars
extern pid_t pid;
extern int fd[2];
extern sem_t * sem;

int log_message(){
    //child process
    close(fd[WRITE_END]);
    int bytesread;
    char buffer[BUFSIZ];

    char *message = buffer;


    printf("logger.c: Entered logger\n");
    // Loop while not end of file or not a read error
    while((bytesread = read( fd[READ_END], buffer, BUFSIZ)) != 0) {

        if ( bytesread > 0 ) { // more data


            do{
                if (*message==0) {
                    if (*(message+1)==0) break;
                    message++;
                }
                printf("logger.c: Child received the word: '%s'\n", message);
                while (*message) message++;
            }
            while (1);

/*
            int j = 0;
            for (int i = 0; i < BUFSIZ; i++) {
                message[i - j] = buffer[i];
                if (buffer[i] == '\0') {
                    printf("logger.c: Child received the word: '%s'\n", message);

                    message[i] = 'x';
                    buffer[i] = 'x';
                    j = i + 1;
                }
            }
            */
        }
    }
    // tell parent that the message reading is over
    close(fd[READ_END]);
    sem_post(sem);
    kill(getpid(), SIGSEGV);
    return 0;
}
