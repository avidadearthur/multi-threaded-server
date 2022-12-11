/**
 * \author Arthur Tavares Quintao
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>

#define PORT 5678   // default port number for testing
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;
typedef time_t sensor_ts_t;         // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

typedef struct {
    sensor_id_t id;
    sensor_value_t value;
    sensor_ts_t ts;
} sensor_data_t;

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
extern int log_messages();

/**
 *
 */
extern int write_to_pipe(char *message, int arg);

#endif /* _CONFIG_H_ */
