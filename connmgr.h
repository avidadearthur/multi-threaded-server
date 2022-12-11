/**
 * \author Arthur Tavares Quintao
 */

void *client_manager(void *client);

/** Connection manager will listen on a  TCP socket and
 * will spawn a client manager for each new connection */
void *connection_manager(void *port);

