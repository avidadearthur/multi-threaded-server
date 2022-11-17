/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "sensor_db.h"

#define SIZE 25
#define READ_END 0
#define WRITE_END 1


/**
 * There will be two processes the parent that handles
 * the file I/O from the sensors like in exercise 1
 * will log the changes made and the child process
 * by the parent in the gateway.log
 */

int main(void){

    int status;
    int wlog_event; // write log event 0 -> 3 code number
    int rlog_event; // read log event
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

    // parent process
    /*
     * log-events to test:
     *
     *    TODO: 1 - A new csv file is created or an existing file has been opened.
     *    TODO: -1 - An error occurred when opening the csv file.
     *    TODO: 2 - Data insertion succeeded.
     *    TODO: -2 - An error occurred when writing to the csv file.
     *    TODO: 3 - The csv file has been closed.
     *    TODO: -3 - An error occurred when closing the csv file.
     **/

    if (pid > 0){ // parent process
        close(fd[READ_END]);

        // start writing to sensor db process
        FILE * db;
        db = open_db("sensor_db.txt", true);
        if(!db){
            wlog_event = -1; // An error occurred when opening the csv file.
        }
        else{
            wlog_event = 1; // A new csv file is created or an existing file has been opened.
        }

        // --------------------DB Open Event----------------------------//
        write(fd[WRITE_END], &wlog_event, sizeof(wlog_event));

        int err = insert_sensor(db,2,11.11,time ( NULL ));

        if(err == 0){
            wlog_event = 2; // Data insertion succeeded.
        }
        else if(err == -1){
            wlog_event = -2; // An error occurred when writing to the csv file.
        }

        // --------------------Insert Sensor 1 Event--------------------//
        write(fd[WRITE_END], &wlog_event, sizeof(wlog_event));

        err = insert_sensor(NULL,3,15.13,time ( NULL ));

        if(err == 0){
            wlog_event = 2; // Data insertion succeeded.
        }
        else if(err == -1){
            wlog_event = -2; // An error occurred when writing to the csv file.
        }

        // --------------------Fail Insert Sensor 3 Event---------------//
        write(fd[WRITE_END], &wlog_event, sizeof(wlog_event));

        err = insert_sensor(db,3,15.13,time ( NULL ));

        if(err == 0){
            wlog_event = 2; // Data insertion succeeded.
        }
        else if(err == -1){
            wlog_event = -2; // An error occurred when writing to the csv file.
        }

        // --------------------Insert Sensor 3 Event--------------------//
        write(fd[WRITE_END], &wlog_event, sizeof(wlog_event));


        err =  close_db(db);
        if(err == 0){
            wlog_event = 3; // The csv file has been closed.
        }
        else if(err == -1){
            wlog_event = -3; // An error occurred when closing the csv file.
        }
        // --------------------Close Open Event----------------------------//
        write(fd[WRITE_END], &wlog_event, sizeof(wlog_event));

        close(fd[WRITE_END]);

        //Parent waits process pid (child)
        waitpid(pid, &status, 0);
        //Option is 0 since I check it later

        if (WIFSIGNALED(status)){
            printf("Error %d \n", status);
        }
        else if (WEXITSTATUS(status)){
            printf("Exited Normally\n");
        }
        //To Here and see the difference
        //printf("Parent process id: %d\n", getpid());
        //printf("pid seen from Parent process: %d\n", pid);
    }

    // child process
    else{
        close(fd[WRITE_END]);

        // handle log event
        FILE * log;
        log = fopen("gateway.log", "a");

        // read until eof
        while (read(fd[READ_END], &rlog_event, sizeof(rlog_event)) > 0) {

            printf("Child reads %d \n", rlog_event);
            insert_log(log, rlog_event);

        }


        fclose(log);
        close(fd[READ_END]);

        // pid will always be 0 in te child process
        //printf("Child process id: %d\n", getpid());
        //printf("pid seen from Child process: %d\n", pid);
        exit(EXIT_SUCCESS);
    }
    return 0;
}