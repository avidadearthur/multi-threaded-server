/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
    db = NULL;
    // trying to insert the sensor before the file is created
    // will not log anything, there's no child - solve later
    insert_sensor(db,99,35.13,time ( NULL )); // An error occurred when writing to the csv file.
    db = open_db("sensor_db.txt", true); // A new csv file is created or an existing file has been opened.
    insert_sensor(db,3,11.11,time ( NULL )); // Data insertion succeeded.
    insert_sensor(db,5,19.13,time ( NULL )); // Data insertion succeeded.
    insert_sensor(db,7,29.16,time ( NULL )); // Data insertion succeeded.
    close_db(db); // The csv file has been closed.

    db = NULL;
    close_db(db); // The csv file has been closed.
    // trying to insert the sensor before the file is created
    // will not log anything, there's no child - solve later
    insert_sensor(db,77,15.13,time ( NULL )); // An error occurred when writing to the csv file.
    db = open_db("sensor_db.txt", true); // A new csv file is created or an existing file has been opened.
    insert_sensor(db,3,15.13,time ( NULL )); // Data insertion succeeded.
    sleep(1); // test different input speeds
    insert_sensor(db,1,16.13,time ( NULL )); // Data insertion succeeded.
    sleep(1);
    insert_sensor(db,1,13.18,time ( NULL )); // Data insertion succeeded.
    close_db(db); // The csv file has been closed.

    return 0;
}