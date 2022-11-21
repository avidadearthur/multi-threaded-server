/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include "messenger.h"
#include "receiver.h"


#define READ_END 0
#define WRITE_END 1

// global vars
pid_t pid;
int fd[2];
sem_t * sem; // We want a semaphore in shared memory using a pointer instead

// creates child process that runs the receiver.c
int spawn_logger() {
    if (pipe(fd) == -1) {
        printf("messenger.c: Pipe failed\n");
        return -1;
    }

    // from: https://stackoverflow.com/questions/5290985/what-happens-when-a-process-enters-a-semaphore-critical-section-and-sleeps
    // Put semaphore in shared memory so that it can be accessed by child process
    sem = mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(sem,1,0);

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
        // go back to main
        return 0;
    }
    // child process
    // spawn the receiver process
    log_message();
    // the receiver process will never reach this point
    return 0;
}

int send_message(char message[]){
    // do something with the message
    // e.g. write in a file
    printf("messenger.c: Message by parent sent: %s \n", message);

    //write pointer to string to the pipe
    write(fd[WRITE_END], message, strlen(message)+1);

    return 0;
}

int kill_child(){
    // wait till semaphore is available
    close(fd[WRITE_END]);
    sem_wait(sem);
    kill(pid, SIGKILL);
    return 0;
}