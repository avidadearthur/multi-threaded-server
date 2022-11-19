/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <string.h>
#include "sensor_db.h"

#define READ_END 0
#define WRITE_END 1

/**
 * There will be two processes the parent that handles
 * the file I/O from the sensors like in exercise 1
 * will log the changes made and the child process
 * by the parent in the gateway.log
 */

int main(void){
    // start writing to sensor db process
    FILE * db;
    db = open_db("sensor_db.txt", true); // A new csv file is created or an existing file has been opened.
    insert_sensor(db,3,15.13,time ( NULL ));
    close_db(db); // The csv file has been closed.
    db = NULL;
    db = open_db("sensor_db.txt", true); // A new csv file is created or an existing file has been opened.
    insert_sensor(db,3,15.13,time ( NULL ));
    insert_sensor(db,1,11.13,time ( NULL ));
    insert_sensor(db,1,11.13,time ( NULL ));
    close_db(db); // The csv file has been closed.

    return 0;
}