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
int fd[2];
sem_t * sem; // We want a semaphore in shared memory using a pointer instead
extern int* line_count;

FILE * open_db(char * filename, bool append){ // parent process
    // spawns the logger process (child)
    spawn_logger();
    char *log_event_message;

    FILE *db;
    if (append) {
        db = fopen(filename, "a");
        log_event_message = "A new csv file is created or an existing file has been opened.";
        // --------------------DB Open Event-----------------//
        write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);
    } else {
        db = fopen(filename, "w+");
        log_event_message = "A new csv file is created or an existing file has been opened.";
        // --------------------DB Open Event-----------------//
        write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);
    }

    return db;
}

int insert_sensor(FILE * db, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){ // parent process

    char *log_event_message;
    if (!db) {
        //printf("sensor_db.c: !f FILE log is : %p \n", db);
        log_event_message = "An error occurred when writing to the csv file.";
        // --------------------Fail Insert Sensor Event---------------//
        // trying to insert the sensor before the file is created
        // will not log anything, there's no child
        // but it will work if you have other files open - solve later
        write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);

        return -1;
    }
    //printf("sensor_db.c: FILE log is: %p \n", &db);

    struct tm *tsm = gmtime(&ts);
    char timestamp[256];
    strftime(timestamp, sizeof(timestamp), "%F %T", tsm);
    fprintf(db, "%d, %f, %s\n", id, value, timestamp);

    log_event_message = "Data insertion succeeded.";
    // --------------------Insert Sensor Event---------------//
    write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);

    return 0;

}

int close_db(FILE * f){ // parent process

    char *log_event_message;
    if (!f) {
        log_event_message = "An error occurred when closing the csv file.";
        // ------------------DB Close fail Event-----------------//
        write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);

        return -1;
    }
    fclose(f);

    log_event_message = "The csv file has been closed.";
    // ------------------DB Close Event-----------------//
    write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);

    close(fd[WRITE_END]);
    wait(NULL); // wait for the child to finish

    return 0;
}