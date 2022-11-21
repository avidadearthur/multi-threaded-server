/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
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
    //char message[BUFSIZ];

    printf("logger.c: Entered logger\n");
    // Loop while not end of file or not a read error
    while((bytesread = read( fd[READ_END], buffer, BUFSIZ)) != 0) {

        // parse buffer for '\0' to separate messages
        if ( bytesread > 0 ) {
            printf("logger.c: Entered bytesread > 0 \n");
            // concatenate '\0' at the beginning of the buffer
            char * message = malloc(bytesread);
            message[0] = '\0';

            // parse buffer for '\0'
            message = strtok(buffer, "\0");
            printf("logger.c: Child received the word: '%s'\n", message);

            do{

                message = strchr(message,0) + 1;
                printf("logger.c: Child received the word: '%s'\n", message);


            }
            while (strcmp(message,"\0") > 0);


/*            for(int i = 0; i<BUFSIZ; i++){
                message[i-changer] = buffer[i];
                if(buffer[i] == '\0'){
                    printf("logger.c: Child received the word: '%s'\n", message);
                    message[i] =  ' ';
                    buffer[i] = ' ';
                    changer = i+1;
                }
            }
            strcpy(buffer, "");
            for(int i = 0; i<BUFSIZ; i++){
                buffer[i] = ' ';
            }
        }
        else { //read error
            perror("logger.c: read()");
            exit(4);
        }*/
    }
/*    while ((bytesread = read( fd[READ_END], buffer, BUFSIZ)) != 0){
        if ( bytesread > 0 ) { // more data
            buffer[bytesread] = '\0';
            printf("logger.c: Child received the word: '%s'\n", buffer);
            fflush(stdout);
            // tell parent that the message reading is over
            sem_post(sem);
        }
        else { //read error
            perror("logger.c: read()");
            exit(4);
        }*/
    }
    // tell parent that the message reading is over
    sem_post(sem);
    close(fd[READ_END]);
    exit(0);
}