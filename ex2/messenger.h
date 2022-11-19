/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>

/**
 * This functions opens a pipe and sends a message.
 *
 * \param message : string to be sent
 * \return integer error code
 */
int send_message(char message[]);