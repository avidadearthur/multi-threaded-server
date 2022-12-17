/**
 * \author Arthur Tavares Quintao
 */

#include "connmgr.h"
#include "datamgr.h"
#include "sbuffer.h"

/** Global vars*/
extern int fd[2]; // file descriptor for the pipe
extern pthread_mutex_t pipe_mutex;
extern sbuffer_t *shared_buffer;

sensor_id_t *seen_nodes;
int seen_nodes_size;


bool sensor_id_in_array(sensor_id_t id, sensor_id_t *pInt, int size);

/** Parent process thread */

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
    // --------------------Max connections reached Event-----------------------//
    message = "Max connections reached. Server is closing.";
    write_to_pipe(message, -1);
    // ------------------------------------------------------------------------//

    if (tcp_close(&server) != TCP_NO_ERROR){
        // ----------------Connection manager thread start Event---------------//
        message = "Server failed to close TCP port.";
        write_to_pipe(message, -1);
        // --------------------------------------------------------------------//
        exit(EXIT_FAILURE);
    }
    else{
        // ------------Connection manager thread start Event-------------------//
        message = "Server closed TCP port.";
        write_to_pipe(message, -1);
        // --------------------------------------------------------------------//

        // Add dummy data to buffer to signal end of file
        sensor_data_t sensor_data;
        sensor_data.id = 0;
        sensor_data.value = 0;
        sensor_data.ts = 0;
        sbuffer_insert(shared_buffer, &sensor_data);
    }
    // ---------------Connection manager thread start Event--------------------//
    message = "Connection manager thread will exit.";
    write_to_pipe(message, -1);
    // ------------------------------------------------------------------------//
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

            // Check if sensor ID is valid (it is in the room_sensor.map file)
            // and if it has not already been registered
            if (datamgr_get_sensor_by_id(data.id) == NULL) {
                // ------------------Invalid sensor ID Event----------------------//
                message = "Invalid sensor ID.";
                write_to_pipe(message, data.id);
                // --------------------------------------------------------------//
                result = TCP_CONNECTION_CLOSED;
                break;
            }
            else if (sensor_id_in_array(data.id, seen_nodes, seen_nodes_size)) {
                // ------------------Duplicate sensor ID Event--------------------//
                message = "Duplicate sensor ID.";
                write_to_pipe(message, data.id);
                // --------------------------------------------------------------//
                result = TCP_CONNECTION_CLOSED;
                break;
            }
            else {
                // --------------------New client connection Event-----------------//
                message = "New connection from sensor %d.";
                write_to_pipe(message, data.id);
                // ----------------------------------------------------------------//
                seen_nodes_size++;
                seen_nodes = realloc(seen_nodes, seen_nodes_size * sizeof(sensor_id_t *));
                // free memory if realloc fails
                if (seen_nodes == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                seen_nodes[seen_nodes_size - 1] = data.id;
            }

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
            // create sensor_data_t
            sensor_data_t sensor_data;
            sensor_data.id = data.id;
            sensor_data.value = data.value;
            sensor_data.ts = data.ts;
            // insert sensor_data into buffer
            sbuffer_insert(shared_buffer, &sensor_data);

            // for testing purposes
            // format sensor_data.ts to readable time
            time_t time = sensor_data.ts;
            struct tm *timeinfo = localtime(&time);
            char timestamp[80];
            strftime(timestamp, 80, "%F %T", timeinfo);

            printf("connmgr.c: data.id: %d, data.value: %f, data.ts: %s \n", data.id, data.value, timestamp);
            //printf("sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", data.id, data.value, (long int) data.ts);
        }
    } while (result == TCP_NO_ERROR);

    if (result == TCP_CONNECTION_CLOSED){
        // ------------------Client closing connection Event------------------//
        message = "Sensor node %d has closed connection.";
        write_to_pipe(message, data.id);
        // -------------------------------------------------------------------//
    }
    else {
        printf("Error occurred on connection to peer\n");
    }
    tcp_close(&client_sock);

    pthread_exit(NULL);
}

bool sensor_id_in_array(sensor_id_t id, sensor_id_t *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == id) {
            return true;
        }
    }
    return false;
}
