/**
 * \author Arthur Tavares Quintao
 */

#include "sensor_db.h"
#include "sbuffer.h"

/** Global vars*/
extern int fd[2]; // file descriptor for the pipe
extern sbuffer_t *shared_buffer;

/** Parent process thread */

void * storage_manager(void *pVoid){

    FILE * db = open_db("data.csv", false);

    sensor_data_t data;
    do{
        // read data from buffer
        if (sbuffer_remove(shared_buffer, &data) == 0) { // SBUFFER_SUCCESS 0
            insert_sensor(db, data.id, data.value, data.ts);
            printf("data.id: %d, data.value: %f, data.ts: %ld \n", data.id, data.value, data.ts);
        }
    }
    while(data.id != 0);

    close_db(db);

    return NULL;
}

FILE * open_db(char * filename, bool append){ // parent process
    char *message;

    FILE *db;
    if (append) {
        db = fopen(filename, "a");
        // --------------------DB Open append Event----------------------------------//
        message = "An existing file has been opened.";
        write_to_pipe(message, -1);
        // --------------------------------------------------------------------------//
    } else {
        db = fopen(filename, "w");
        // --------------------DB Open create Event----------------------------------//
        message = "A new csv file has been opened.";
        write_to_pipe(message, -1);
        // --------------------------------------------------------------------------//
    }

    return db;
}

int insert_sensor(FILE * db, sensor_id_t id, sensor_value_t value, sensor_ts_t ts){ // parent process
    char *message;

    if (!db) {
        // --------------------Fail Insert Sensor Event-------------------------------//
        message = "An error occurred when writing to the csv file.";
        write_to_pipe(message, -1);
        // --------------------------------------------------------------------------//

        return -1;
    }

    struct tm *tsm = gmtime(&ts);
    char timestamp[256];
    strftime(timestamp, sizeof(timestamp), "%F %T", tsm);
    fprintf(db, "%d, %f, %s\n", id, value, timestamp);

    // --------------------Insert Sensor Event succeeded----------------------------------//
    message = "Data insertion from sensor %d succeeded.";
    write_to_pipe(message, id);

    return 0;

}

int close_db(FILE * f){ // parent process
    char *message;
    if (fclose(f) != 0) {
        message = "An error occurred when closing the csv file.";
        // --------------------Fail DB Close Event----------------------------------//
        write_to_pipe(message, -1);
        // --------------------------------------------------------------------------//
        return -1;
    }
    message = "The csv file has been closed.";
    // --------------------DB Close Event------------------------------------//
    write_to_pipe(message, -1);
    // ---------------------------------------------------------------------//
    return 0;

}

