/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "config.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "sensor_db.h"
#include "connmgr.h"


/** Global vars*/
int fd[2]; // file descriptor for the pipe
pthread_mutex_t pipe_mutex = PTHREAD_MUTEX_INITIALIZER;

/** Parent process treads */
//void *storage_manager(void);
//void *data_manager(void);
//void *connection_manager(void *port);
//void *client_manager(void *client);

/**
 * The sensor gateway has a main process and a logger (child) process.
 * The main runs three threads at startup:
 * 1. connection manager thread
 * 2. data manager thread
 * 3. storage manager thread
 *
 */
int main(int argc, char *argv[]) {
    // TODO: check user port input for errors
    int port_number = PORT;

    // check command line arguments
    if (argc == 1) {
        fprintf(stderr, "Using default port %d\n", port_number);
    }
    // check for only one command line argument
    else if (argc > 2) {
        fprintf(stderr, "Usage: %s port (default port is %d)\n", argv[0], PORT);
        exit(EXIT_FAILURE);
    }
    else {
        // convert argv[1] to integer with strtol
        port_number = strtol(argv[1], NULL, 10);
        if (port_number < 1024 || port_number > 65535) {
            fprintf(stderr, "Invalid port number \n");
            fprintf(stderr, "Usage: %s port (default port is %d)\n", argv[0], PORT);
            exit(EXIT_FAILURE);
        }
    }

    // TODO: fork in two processes: main and logger
    // create the pipe
    if (pipe(fd) == -1) {
        printf("main: Pipe failed\n");
        return -1;
    }

    switch (fork()) {
        case -1:
            perror("main: Fork failed\n");
            exit(EXIT_FAILURE);

        case 0:
            // child process
            close(fd[WRITE_END]);
            log_messages();
            close(fd[READ_END]);
            exit(EXIT_SUCCESS);
        default:
            // parent process
            close(fd[READ_END]);

            // TODO: run connection manager thread
            pthread_t connection_manager_thread;
            pthread_create(&connection_manager_thread, NULL, connection_manager, &port_number);

            // TODO: run data manager thread

            // TODO: run storage manager thread

            // TODO: close all threads and exit
            // wait for the connection manager thread to finish
            pthread_join(connection_manager_thread, NULL);
            // wait for the child process to finish
            close(fd[WRITE_END]);
            wait(NULL);
    }
}

/** Logger functions */

int write_to_pipe(char *message, int arg) {
    char *log_event_message;

    // lock the pipe
    pthread_mutex_lock(&pipe_mutex);
    if (arg != -1){
        log_event_message = (char *) malloc(sizeof(char) * 100);

        sprintf(log_event_message, message, arg);
        log_event_message = realloc(log_event_message, strlen(log_event_message)+1); // trim the string

        write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);
        free(log_event_message);
    }
    else {
        write(fd[WRITE_END], message, strlen(message)+1);
    }

    // unlock the pipe
    pthread_mutex_unlock(&pipe_mutex);

    return 0;
}


int log_messages(){ //child process
    printf("sensor_db.c: child process log_message started\n");
    char buffer[BUFSIZ];
    char message[BUFSIZ];
    int line_count = 0;
    char ts[64];

    // open logger and count number of lines
    FILE *logger;
    logger = fopen("gateway.log", "w");
    fprintf(logger,"start of test\n");

    // clear the buffer
    memset(buffer, 0, BUFSIZ);

    // read is looping over every byte in the pipe
    // and is a blocking call until there's something to read
    // or the pipe is closed
    while(read( fd[READ_END], buffer, BUFSIZ) > 0 ) {
        int j = 0;
        memset(message, ' ', BUFSIZ); // make sure its empty
        // look for null terminator in buffer
        for(int i= 0; i < BUFSIZ; i++){
            // copy every byte until the null terminator
            message[i-j] = buffer[i];
            if(buffer[i] == '\0'){
                if(message[0] != '\0'){
                    // <sequence number>
                    line_count++;
                    // <timestamp>
                    time_t t = time(NULL);
                    struct tm *tm = localtime(&t);
                    strftime(ts, sizeof(ts), "%F %T", tm);
                    // combine <sequence number> <timestamp> <log-event info log_message>
                    printf("sensor_db.c: full log message: %d, %s, %s\n", line_count, ts, message);
                    fprintf(logger,"%d, %s, %s\n", line_count, ts, message);
                }
                buffer[i] = ' ';
                // reset j such that i - j is 0 for the next char of the buffer
                j = i + 1;
            }
        }
        memset(message, ' ', BUFSIZ); // clear message
    }
    fprintf(logger,"end of test\n");
    fclose(logger); // for now, we'll open and close the log file every time

    return 0;
}