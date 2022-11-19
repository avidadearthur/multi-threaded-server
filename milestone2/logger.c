#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#include "sensor_db.h"

#define READ_END 0
#define WRITE_END 1



int logger(int fd[]){
    int n;
    close(fd[WRITE_END]); // close the write direction because you just want to read it

    while (read(fd[READ_END], &n, sizeof(int)) > 0){

        char *log_event;

        // <log-event info message>
        printf("Received n = %d\n", n);
        fflush(stdout);
        read(fd[READ_END], &log_event, sizeof(int) * n);

        printf("Child reads %s \n", log_event);
        // Log
        // handle log event
        FILE * log;
        if(!log) return 7;
        log = fopen("gateway.log", "a");

        printf("print from insert_log FILE f is: %p \n", &log);
        printf("print from insert_log int log_event is: %s \n", log_event);


        // <sequence number>
        int count = 0;
//        char c;
//        // Extract /n from file and store in character c
//        for (c = getc(log); c != EOF; c = getc(log))
//            if (c == '\n') // Increment count if this character is newline
//                count = count + 1;
        printf("print from insert_log current long line count is: %d \n", count);

        // <timestamp>
        // num of sec since 01/01/70
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char ts[64];
        strftime(ts, sizeof(ts), "%F %T", tm);
        printf("print from insert_log ts is: %s \n", ts);

        // combine <sequence number> <timestamp> <log-event info message>
        // uncomment when things are working
        fprintf(log,"%d, %s, %s\n", count, ts, log_event);
        fflush(log);
        printf("print to log: %d, %s, %s\n", count, ts, log_event);
        fflush(stdout);
        fclose(log); // for now, we'll open and close the log file every time

    }
    close(fd[READ_END]);
    return 0;
}
