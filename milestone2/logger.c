#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "logger.h"
#include "sensor_db.h"


#define READ_END 0
#define WRITE_END 1

// global vars
extern pid_t pid;
extern int fd[2];
extern sem_t * sem;


int spawn_logger() { // spawns the logger process (child) that runs the logger.c
    if (pipe(fd) == -1) {
        printf("messenger.c: Pipe failed\n");
        return -1;
    }

    // from: https://stackoverflow.com/questions/5290985/what-happens-when-a-process-enters-a-semaphore-critical-section-and-sleeps
    // Put semaphore in shared memory so that it can be accessed by child process
    sem = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(sem,1,0); // init semaphore such that wait(sem) will block parent process

    // fork the child
    pid = fork();
    // the parent process will have the pid of the child but the child process
    // will not  be aware of that pid, so it only enters the else clause

    if (pid < 0) { // fork error
        printf("messenger.c: fork failed");
        return -1;
    }
    if (pid > 0) { // parent process
        //  will write to the pipe
        close(fd[READ_END]); // do just once!!
        // go back to open_db
        return 0;
    }
    // child process
    // spawn the receiver process
    log_message();
    // the receiver process will never reach this point
    return 0;
}

int log_message(){ //child process

    close(fd[WRITE_END]);
    int bytesread;
    // char log_message[BUFSIZ];
    char buffer[BUFSIZ];

    int count;
    char ts[64];

    FILE * log;
    log = fopen("gateway.log", "a");
    if(log) printf("logger.c: FILE log is: %p \n", &log);
    if(!log) return 7;

    // from: https://stackoverflow.com/questions/5290985/what-happens-when-a-process-enters-a-semaphore-critical-section-and-sleeps
    // Loop while not end of file or not a read error
    while ((bytesread = read( fd[READ_END], buffer, BUFSIZ)) != 0){

        if ( bytesread > 0 ) { // more data
            printf("logger.c: Entered bytesread > 0 \n");
            // concatenate '\0' at the beginning of the buffer
            char * message = malloc(bytesread + 1);
            message[0] = '\0';

            // parse buffer for '\0'
            message = strtok(buffer, "\0");
            printf("logger.c: Child received the word: '%s'\n", message);
            // <sequence number> // solve later
            count = 0;
            // <timestamp> // num of sec since 01/01/70
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            strftime(ts, sizeof(ts), "%F %T", tm);
            // combine <sequence number> <timestamp> <log-event info log_message>
            printf("logger.c: full log message: %d, %s, %s\n", count, ts, message);
            fprintf(log,"%d, %s, %s\n", count, ts, message);


            do{
                message = strchr(message,0) + 1;
                printf("logger.c: Child received the word: '%s'\n", message);
                // <sequence number> // solve later
                count = 0;
                // <timestamp> // num of sec since 01/01/70
                time_t t = time(NULL);
                struct tm *tm = localtime(&t);
                strftime(ts, sizeof(ts), "%F %T", tm);
                // combine <sequence number> <timestamp> <log-event info log_message>
                printf("logger.c: full log message: %d, %s, %s\n", count, ts, message);
                fprintf(log,"%d, %s, %s\n", count, ts, message);
            }
            while (strcmp(message,"\0") > 0);
        }
        else { //read error
            perror("logger.c: read()");
            exit(4);
        }
    }
    sem_post(sem);
    fclose(log); // for now, we'll open and close the log file every time
    close(fd[READ_END]);
    exit(0);
}

int kill_logger(){
    close(fd[WRITE_END]);
    sem_wait(sem);
    kill(pid, SIGKILL);

    return 0;
}
