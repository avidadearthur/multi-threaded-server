/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include "sensor_db.h"
#include <semaphore.h>



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
int logger(int fd[]);


/**
 * Kill child process
 */
int kill_logger();