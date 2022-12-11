/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "config.h"
#include "lib/tcpsock.h"
#include "lib/dplist.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "sensor_db.h"

#define PORT 5678   // default port number for testing
#define MAX_CONN 3  // state the max. number of connections the server will handle before exiting

/** Global vars*/
int fd[2]; // file descriptor for the pipe
bool logger_running = false; // flag to check if the logger is running
pthread_mutex_t pipe_mutex = PTHREAD_MUTEX_INITIALIZER;

/** Parent process treads */
//void *storage_manager(void);
//void *data_manager(void);

void *connection_manager(void *port);

/** Connection manager will listen on a  TCP socket and
 * will spawn a client manager for each new connection */
void *client_manager(void *client);

/**
 * The sensor gateway has a main process and a logger (child) process.
 * The main runs three threads at startup:
 * 1. connection manager thread
 * 2. data manager thread
 * 3. storage manager thread
 *
 * The connection manager thread is responsible for:
 * 1. listens for new connections from sensors on a TCP socket
 * 2. creates a new thread for each new connection
 *    2.1. the new thread is responsible for reading the data from the sensor
 *
 */
int main(int argc, char *argv[]) {
    // TODO: check user port input for errors
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
        if (port_number < 1024 || port_number > 65535) {
            fprintf(stderr, "Invalid port number \n");
            fprintf(stderr, "Usage: %s port (default port is %d)\n", argv[0], PORT);
            exit(EXIT_FAILURE);
        }
    }

    // TODO: fork in two processes: main and logger
    // create the pipe
    if (pipe(fd) == -1) {
        printf("main: Pipe failed\n");
        return -1;
    }

    switch (fork()) {
        case -1:
            perror("main: Fork failed\n");
            exit(EXIT_FAILURE);
        case 0:
            // child process
            close(fd[WRITE_END]);
            log_messages();

        default:
            // parent process
            close(fd[READ_END]);

            // TODO: run connection manager thread
            pthread_t connection_manager_thread;
            pthread_create(&connection_manager_thread, NULL, connection_manager, &port_number);

            // TODO: run data manager thread

            // TODO: run storage manager thread

            // TODO: close all threads and exit
            // wait for the connection manager thread to finish
            pthread_join(connection_manager_thread, NULL);
    }

}


/** helper function */

int write_to_pipe(char *message, int arg) {
    char *log_event_message;

    if (arg != -1){
        log_event_message = (char *) malloc(sizeof(char) * 100);

        sprintf(log_event_message, message, arg);
        log_event_message = realloc(log_event_message, strlen(log_event_message)+1); // trim the string

        write(fd[WRITE_END], log_event_message, strlen(log_event_message)+1);
    }
    else {
        write(fd[WRITE_END], message, strlen(message)+1);
    }

    return 0;
}


/** Parent process treads */

void *connection_manager(void *port) {
    tcpsock_t *server, *client;
    pthread_t tid;
    char *message;
    int port_number = *(int *) port;
    pthread_t connections[MAX_CONN];
    int conn_count = 0;

    // --------------------Connection manager thread start Event-----------------//
    message = "Connection manager thread started.";
    write_to_pipe(message, -1);
    // --------------------------------------------------------------------------//
    if (tcp_passive_open(&server, port_number) != TCP_NO_ERROR) exit(EXIT_FAILURE);

    // ------------------------------Starting server Event-----------------------//
    message = "Test server started at port %d.";
    write_to_pipe(message, port_number);
    // --------------------------------------------------------------------------//

    do {
        if (tcp_wait_for_connection(server, &client) != TCP_NO_ERROR) exit(EXIT_FAILURE);
        printf("connection_manager: Incoming client connection.\n");
        pthread_create(&tid, NULL,  client_manager, client);
        connections[conn_count] = tid;
        conn_count++;

    } while (conn_count < MAX_CONN);

    // For each connection, wait for the thread to finish, if during test you kill the thread with ctrl-c
    // the loop bellow will join a thread that already finished, this is not a problem.
    for (int i = 0; i < conn_count; i++) {
        pthread_join(connections[i], NULL);
    }
    // --------------------Connection manager thread start Event-----------------------//
    message = "Max connections reached. Server is closing.";
    write_to_pipe(message, -1);
    // -------------------------------------------------------------------------------//
    // --------------------Connection manager thread start Event-----------------------//
    message = "Connection manager thread will exit.";
    write_to_pipe(message, -1);
    // -------------------------------------------------------------------------------//
    pthread_exit(NULL);

}

void *client_manager(void *client){

    bool new_connection = true;
    char *message;
    sensor_data_t data;
    int bytes, result;
    tcpsock_t *client_sock = (tcpsock_t *) client;

    do {
        // read sensor ID
        bytes = sizeof(data.id);
        tcp_receive(client_sock, (void *) &data.id, &bytes);

        if (new_connection) {
            // -----------------------New client connection Event-------------------------//
            message = "New connection from sensor %d.";
            write_to_pipe(message, data.id);
            // ---------------------------------------------------------------------------//

            new_connection = false;
        }

        // read temperature
        bytes = sizeof(data.value);
        tcp_receive(client_sock, (void *) &data.value, &bytes);
        // read timestamp
        bytes = sizeof(data.ts);
        result = tcp_receive(client_sock, (void *) &data.ts, &bytes);
        if ((result == TCP_NO_ERROR) && bytes) {
            // this will go into the sbuffer
            printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value,
                   (long int) data.ts);
        }
    } while (result == TCP_NO_ERROR);
    if (result == TCP_CONNECTION_CLOSED){
        // log this into the log file
        // ----------------------------Client closing connection Event-------------------//
        message = "Sensor node %d has closed connection.";
        write_to_pipe(message, data.id);
        // ------------------------------------------------------------------------------//
    }
    else {
        printf("Error occurred on connection to peer\n");
    }
    tcp_close(&client_sock);

    pthread_exit(NULL);
}
