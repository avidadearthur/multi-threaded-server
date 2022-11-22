#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "logger.h"
#include "sensor_db.h"


#define READ_END 0
#define WRITE_END 1

// global vars
extern pid_t pid;
extern int fd[2];
extern sem_t * sem;


int spawn_logger() { // spawns the logger process (child) that runs the logger.c

    // from: https://stackoverflow.com/questions/5290985/what-happens-when-a-process-enters-a-semaphore-critical-section-and-sleeps
    // Put semaphore in shared memory so that it can be accessed by child process
    // sem = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // sem_init(sem,1,0); // init semaphore such that wait(sem) will block parent process

    if (pipe(fd) == -1) {
        printf("messenger.c: Pipe failed\n");
        return -1;
    }

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

    //child process
    close(fd[WRITE_END]);
    char buffer[BUFSIZ];
    char message[BUFSIZ];
    //char *message;
    int count;
    char ts[64];

    FILE * log;
    log = fopen("gateway.log", "a");

    // read is looping over every byte in the pipe
    // and is a blocking call until there's something to read
    // or the pipe is closed
    // sleep(1); to test that the parsing works
    while(read( fd[READ_END], buffer, BUFSIZ) > 0 ) {
        int j = 0;
        memset(message, ' ', BUFSIZ); // make sure its empty
        // look for null terminator in buffer
        for(int i= 0; i < BUFSIZ; i++){
            // copy every byte until the null terminator
            message[i-j] = buffer[i];
            if(buffer[i] == '\0'){
                if(message[0] != '\0'){
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
                buffer[i] = ' ';
                // reset j such that i - j is 0 for the next char of the buffer
                j = i + 1;
            }
        }
        memset(message, ' ', BUFSIZ); // clear message
    }
    fclose(log); // for now, we'll open and close the log file every time
    close(fd[READ_END]);
    kill(getpid(), SIGSEGV);

    return 0;
}
