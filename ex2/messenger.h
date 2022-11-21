/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>

/**
 * Spawn a child process that will run the logger function in milestone.
 * The child process will read the events through a pipe that is created
 * when the sensor_db file is open.
 *
 * \return integer error code
 * */
int spawn_logger();

/**
 * This functions opens a pipe and sends a message.
 *
 * \param message : string to be sent
 * \return integer error code
 */
int send_message(char message[]);


/**
 * Kill child process
 */
int kill_child();