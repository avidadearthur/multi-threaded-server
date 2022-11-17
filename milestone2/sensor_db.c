/**
 * \author Arthur Tavares Quintao
 */

#include "sensor_db.h"

FILE * open_db(char * filename, bool append){
    FILE * db;
    if(append){
        db = fopen(filename, "a");
    }
    else{
        db = fopen(filename, "w+");
    }
    if (!db)perror("fopen");

    return db;
}

int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){
    if(!f) return -1;
    struct tm *tsm = gmtime(&ts);
    char timestamp[256];
    strftime(timestamp,sizeof(timestamp), "%F %T", tsm);
    fprintf(f,"%d, %f, %s\n", id, value, timestamp);

    return 0;
}

int close_db(FILE * f){
    if(!f) return -1;

    fclose(f);

    return 0;
}

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
int insert_log(FILE * f, int log_event){

    printf("print from insert_log FILE f is: %p \n", &f);
    printf("print from insert_log int log_event is: %d \n", log_event);

    if(!f) return -1;

    // <sequence number>
    int count = 0;
    char c;
    // Extract /n from file and store in character c
    for (c = getc(f); c != EOF; c = getc(f))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    printf("print from insert_log current long line count is: %d \n", count);

    // <log-event info message>
    char *log_event_message;
    if(log_event == 1) log_event_message = "A new csv file is created or an existing file has been opened.";
    if(log_event == -1) log_event_message = "An error occurred when opening the csv file.";
    if(log_event == 2) log_event_message = "Data insertion succeeded.";
    if(log_event == -2) log_event_message = "An error occurred when writing to the csv file.";
    if(log_event == 3) log_event_message = "The csv file has been closed.";
    if(log_event == -3) log_event_message = "An error occurred when closing the csv file.";

    printf("print from insert_log log message is: %s \n", log_event_message);

    // <timestamp>
    // num of sec since 01/01/70
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char ts[64];
    strftime(ts, sizeof(ts), "%F %T", tm);
    printf("print from insert_log ts is: %s \n", ts);

    // combine <sequence number> <timestamp> <log-event info message>
    fprintf(f,"%d, %s, %s\n", count, ts, log_event_message);

    return 0;

}