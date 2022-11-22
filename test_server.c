/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "config.h"
#include "lib/tcpsock.h"
#include <pthread.h>

#define PORT 5678   // default port number for testing
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting


/**
 * Implements a multi-threaded test server (every connection starts a new thread).
 */
void *runner(void *client);

int main(int argc, char *argv[]) {
    tcpsock_t *server, *client;
    pthread_t tid;
    int conn_counter = 0;
    int port_number = PORT;

    // check command line arguments
    if (argc == 1) {
        fprintf(stderr, "Using default port %d\n", port_number);
    }
    // check for only one command line argument
    else if (argc > 2) {
        fprintf(stderr, "Usage: %s port (default port is %d)\n", argv[0], PORT);
        exit(EXIT_FAILURE);
    }
    else {
        // convert argv[1] to integer with strtol
        port_number = strtol(argv[1], NULL, 10);
        if (port_number == 0) {
            fprintf(stderr, "Invalid port number \n");
            fprintf(stderr, "Usage: %s port (default port is %d)\n", argv[0], PORT);
            exit(EXIT_FAILURE);
        }
    }

    printf("Test server is started at port %d \n", port_number);
    if (tcp_passive_open(&server, port_number) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        printf("Incoming client connection\n");
        conn_counter++;

        pthread_create(&tid, NULL,  runner, client);

    } while (conn_counter < MAX_CONN);

    for (int i = 0; i < MAX_CONN; i++) {
        pthread_join(tid, NULL);
    }

    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    return 0;
}

void *runner(void *client){
    sensor_data_t data;
    int bytes, result;
    tcpsock_t *client_sock = (tcpsock_t *) client;
    do {
        // read sensor ID
        bytes = sizeof(data.id);
        result = tcp_receive(client_sock, (void *) &data.id, &bytes);
        // read temperature
        bytes = sizeof(data.value);
        result = tcp_receive(client_sock, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(client_sock, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                   (long int) data.ts);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED)
        printf("Peer has closed connection\n");
    else
        printf("Error occured on connection to peer\n");
    tcp_close(&client_sock);
}




