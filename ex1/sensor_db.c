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