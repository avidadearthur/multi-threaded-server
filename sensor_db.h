/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "config.h"
#include <stdbool.h>

#define READ_END 0
#define WRITE_END 1

/**
 * Operation to open a text file with a given name, and providing an indication
 * if the file should be overwritten if the file already exists or if the data should
 * be appended to the existing file.
 *
 * \param filename : 'sensor_db.txt'
 * \param append : overwrite or not
 * \return FILE pointer
 */
FILE * open_db(char * filename, bool append);

/**
 * Operation to append a single sensor reading to the csv file.
 *
 * File format example:
 *
 *       sensorId    sensorValue     timestamp
 *   l0       1           12           12:00:00
 *   l1       2           10           12:01:34
 *      ...      ...         ...             ...
 *
 * \param f : FILE pointer
 * \param id : sensor id
 * \param value : sensor value
 * \param ts : sensor timestamp
 * \return error code
 */
int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts);

/**
 * Operation to close the csv file.
 *
 * \param f : FILE pointer
 * \return error code
 */
int close_db(FILE * f);


/**
 * Spawn a child process that will run the logger function in milestone.
 * The child process will read the events through a pipe that is created
 * when the sensor_db file is open.
 *
 * \return integer error code
 * */
int spawn_logger();

/**
 * Operation to update the log file. Each line of the log file is structured
 * as follows:  <sequence number> <timestamp> <log-event info message>
 * A log-event contains an ASCII info message describing the type of event.
 *
 * This function runs as a child process reading the events through a pipe
 * that is created when the sensor_db file is open.
 *
 * A few examples of log-events are:
 *     0 - A new csv file is created or an existing file has been opened.
 *     1 - Data insertion succeeded.
 *     2 - An error occurred when writing to the csv file.
 *     3 - The csv file has been closed.
 *
 */
int log_message();