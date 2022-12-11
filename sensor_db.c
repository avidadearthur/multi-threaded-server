/**
 * \author Arthur Tavares Quintao
 */

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "sensor_db.h"

/** Global vars*/
static int *line_count;
extern int fd[2];
extern bool logger_running;

int log_messages(){ //child process
    printf("sensor_db.c: child process log_message started\n");
    char buffer[BUFSIZ];
    char message[BUFSIZ];
    int count = 0;
    char ts[64];

    // Put counter in shared memory so that it can be accessed by child process
    line_count = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, -1, 0);
    (*line_count) = 0;
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
                    (*line_count)++;
                    count = *line_count;
                    // <timestamp>
                    time_t t = time(NULL);
                    struct tm *tm = localtime(&t);
                    strftime(ts, sizeof(ts), "%F %T", tm);
                    // combine <sequence number> <timestamp> <log-event info log_message>
                    printf("sensor_db.c: full log message: %d, %s, %s\n", count, ts, message);
                    fprintf(logger,"%d, %s, %s\n", count, ts, message);
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

    close(fd[READ_END]);

    return 0;
}