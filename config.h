/**
 * \author Arthur Tavares Quintao
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include <time.h>
#include <pthread.h>

#define PORT 5678   // default port number for testing
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting

#define READ_END 0
#define WRITE_END 1

typedef uint16_t sensor_id_t;
typedef double sensor_value_t;
typedef time_t sensor_ts_t;   // UTC timestamp as returned by time() - notice that the size of time_t is different on 32/64 bit machine

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
 * that is created in the main function.
 */
extern int log_messages();

/**
 * Passes the log message to the logger process through a pipe. For the messages
 * that require formatting, the function will use the arguments passed to it.
 *
 * @param message - char * to the message to be logged
 * @param arg - int argument to be used in the formatting of the message
 *
 * @return integer error code
 */
extern int write_to_pipe(char *message, int arg);

#endif /* _CONFIG_H_ */
