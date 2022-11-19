/**
 * \author Arthur Tavares Quintao
 */

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "sensor_db.h"
#include "logger.h"

#define READ_END 0
#define WRITE_END 1

// global vars
pid_t pid;
static int fd[2];

FILE * open_db(char * filename, bool append){
    char *log_event_message;

    // create the pipe
    if (pipe(fd) == -1){
        printf("Pipe failed\n");
        return NULL;
    }
    // fork the child
    pid = fork();
    // the parent process will have the pid of the child but the child process
    // will not  be aware of that pid, so it only enters the else clause

    if (pid < 0){ // fork error
        printf("fork failed\n");
        return NULL;
    }
    if (pid > 0) { // parent process
        close(fd[READ_END]); // do just once!!
        FILE *db;
        if (append) {
            db = fopen(filename, "a");
            log_event_message = "A new csv file is created or an existing file has been opened.";

            // --------------------DB Open Event-----------------//
            int n = strlen(log_event_message) + 1;
            write(fd[WRITE_END], &n, sizeof(int));
            write(fd[WRITE_END], &log_event_message, sizeof(char) * n);

        } else {
            db = fopen(filename, "w+");
        }
        if (!db){
            log_event_message = "An error occurred when opening the csv file.";

            // ------------------DB Open fail Event-----------------//
            int n = strlen(log_event_message) + 1;
            write(fd[WRITE_END], &n, sizeof(int));
            write(fd[WRITE_END], &log_event_message, sizeof(char) * n);

        }
        sleep(0.01); // let parent process wait until parent finishes
        return db;
    }

    // Spawn child and keep it in a loop
    logger(fd);
    exit(0);
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if (pid > 0) { // parent process
        char *log_event_message;
        if (!f){
            log_event_message = "An error occurred when writing to the csv file.";
            // --------------------Fail Insert Sensor Event---------------//
            int n = strlen(log_event_message) + 1;
            write(fd[WRITE_END], &n, sizeof(int));
            write(fd[WRITE_END], &log_event_message, sizeof(char) * n);

            return -1;
        }
        struct tm *tsm = gmtime(&ts);
        char timestamp[256];
        strftime(timestamp, sizeof(timestamp), "%F %T", tsm);
        fprintf(f, "%d, %f, %s\n", id, value, timestamp);
        fflush(f);

        log_event_message = "Data insertion succeeded.";
        // --------------------Insert Sensor Event---------------//
        int n = strlen(log_event_message) + 1;
        write(fd[WRITE_END], &n, sizeof(int));
        write(fd[WRITE_END], &log_event_message, sizeof(char) * n);

        //sleep(0.01); // let parent process wait until parent finishes
        return 0;
    }

    return -1;
}

int close_db(FILE * f){
    if (pid > 0) { // parent process

        char *log_event_message;
        if (!f){
            log_event_message = "An error occurred when closing the csv file.";

            // ------------------DB Close fail Event-----------------//
            int n = strlen(log_event_message) + 1;
            write(fd[WRITE_END], &n, sizeof(int));
            write(fd[WRITE_END], &log_event_message, sizeof(char) * n);

            return -1;
        }

        fclose(f);

        log_event_message = "The csv file has been closed.";

        // ------------------DB Close Event-----------------//
        int n = strlen(log_event_message) + 1;
        write(fd[WRITE_END], &n, sizeof(int));
        write(fd[WRITE_END], &log_event_message, sizeof(char) * n);

        close(fd[WRITE_END]); // child dies

        sleep(0.01); // let parent process wait until parent finishes
        return 0;
    }
    return -1;
}