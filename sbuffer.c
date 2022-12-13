/**
 * \author Arthur Tavares Quintao
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "sbuffer.h"

/**
 * basic node for the buffer, these nodes are linked together to create the buffer
 */
typedef struct sbuffer_node {
    struct sbuffer_node *next;  /**< a pointer to the next node*/
    sensor_data_t data;         /**< a structure containing the data */
} sbuffer_node_t;

/**
 * a structure to keep track of the buffer
 */
struct sbuffer {
    sbuffer_node_t *head;       /**< a pointer to the first node in the buffer */
    sbuffer_node_t *tail;       /**< a pointer to the last node in the buffer */
};


// Declare global mutex and condition variables
pthread_mutex_t mutex;
pthread_cond_t empty, removing;


int sbuffer_init(sbuffer_t **buffer) {
    *buffer = malloc(sizeof(sbuffer_t));
    if (*buffer == NULL) return SBUFFER_FAILURE;
    (*buffer)->head = NULL;
    (*buffer)->tail = NULL;

    // Initialize mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&removing, NULL);

    return SBUFFER_SUCCESS;
}

int sbuffer_free(sbuffer_t **buffer) {
    sbuffer_node_t *dummy;
    if ((buffer == NULL) || (*buffer == NULL)) {
        return SBUFFER_FAILURE;
    }
    while ((*buffer)->head) {
        dummy = (*buffer)->head;
        (*buffer)->head = (*buffer)->head->next;
        free(dummy);
    }
    free(*buffer);
    *buffer = NULL;
    return SBUFFER_SUCCESS;
}

int sbuffer_remove(sbuffer_t *buffer, sensor_data_t *data) {
    if (buffer == NULL) {
        return SBUFFER_FAILURE;
    }

    pthread_mutex_lock(&mutex);

    // Wait until the buffer is nonempty
    while (buffer->head == NULL) {
        pthread_cond_wait(&empty, &mutex);
    }

    // Copy the first item from the buffer
    *data = buffer->head->data;

    if (data->id == 0) {
        // end-of-stream: leave the item in the buffer for other readers to see
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&empty); // Signal that buffer is not empty so other threads can consume this item

        return SBUFFER_NO_DATA;
    } // else remove the item

    sbuffer_node_t *dummy = buffer->head;

    buffer->head = buffer->head->next;
    if (buffer->head == NULL) {
        // buffer is now empty
        buffer->tail = NULL;
    }

    pthread_mutex_unlock(&mutex);

    free(dummy);

    return SBUFFER_SUCCESS;
}

int sbuffer_insert(sbuffer_t *buffer, sensor_data_t *data) {
    sbuffer_node_t *dummy;
    if (buffer == NULL) return SBUFFER_FAILURE;
    dummy = malloc(sizeof(sbuffer_node_t));
    if (dummy == NULL) return SBUFFER_FAILURE;
    dummy->data = *data;
    dummy->next = NULL;

    pthread_mutex_lock(&mutex);
    if (buffer->tail == NULL) // buffer empty (buffer->head should also be NULL
    {
        assert(buffer->head == NULL);
        buffer->head = buffer->tail = dummy;
        pthread_cond_broadcast(&empty); // Signal that buffer is not empty
    } else // buffer not empty
    {
        buffer->tail->next = dummy;
        buffer->tail = buffer->tail->next;
    }
    pthread_mutex_unlock(&mutex);

    return SBUFFER_SUCCESS;
}
