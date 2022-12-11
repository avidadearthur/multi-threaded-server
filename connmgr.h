/**
 * \author Arthur Tavares Quintao
 */

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include "lib/tcpsock.h"
#include "lib/dplist.h"
#include "config.h"

/**
 * The connection manager thread is responsible for:
 * 1. Listening for new connections from sensors on a TCP socket
 * 2. Creating a new thread for each new connection
 *    2.1. the new thread is responsible for reading the data from the sensor
 *
 *    @param client - a double pointer, that will be filled out with the newly created socket for the connection with the client
 *    @return void
 */
void *client_manager(void *client);

/**
 * The connection manager thread is responsible for:
 * 1. Listening for new connections from sensors on a TCP socket
 * 2. Creating a new thread for each new connection
 *    2.1. the new thread is responsible for reading the data from the sensor
 *
 *    @param port the port number to listen for connections
 *    @return void
 */
void *connection_manager(void *port);

