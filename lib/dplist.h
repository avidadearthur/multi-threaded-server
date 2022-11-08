/**
 * \author Arthur Tavares Quintao
 */

#ifndef _DPLIST_H_
#define _DPLIST_H_

typedef enum {
    false, true
} bool; // or use C99 #include <stdbool.h>

/**
 * dplist_t is a struct containing at least a head pointer to the start of the list;
 */
typedef struct dplist dplist_t;

typedef struct dplist_node dplist_node_t;

/* General remark on error handling
 * All functions below will:
 * - use assert() to check if memory allocation was successfully.
 */

/** Create and allocate memory for a new list
 * \param sensor_copy callback function to duplicate 'sensor'; If needed allocated new memory for the duplicated sensor.
 * \param sensor_free callback function to free memory allocated to sensor
 * \param sensor_compare callback function to compare two sensor sensors; returns -1 if x<y, 0 if x==y, or 1 if x>y
 * \return a pointer to a newly-allocated and initialized list.
 */
dplist_t *dpl_create(
        void* (*sensor_copy)(void *sensor),
        void (*sensor_free)(void **sensor),
        int (*sensor_compare)(void *x, void *y)
);

/** Deletes all sensors in the list
 * - Every list node of the list needs to be deleted. (free memory)
 * - The list itself also needs to be deleted. (free all memory)
 * - '*list' must be set to NULL.
 * \param list a double pointer to the list
 * \param free_sensor if true call sensor_free() on the sensor of the list node to remove
 */
void dpl_free(dplist_t **list, bool free_sensor);

/** Returns the number of sensors in the list.
 * - If 'list' is is NULL, -1 is returned.
 * \param list a pointer to the list
 * \return the size of the list
 */
int dpl_size(dplist_t *list);

/** Inserts a new list node containing an 'sensor' in the list at position 'index'
 * - the first list node has index 0.
 * - If 'index' is 0 or negative, the list node is inserted at the start of 'list'.
 * - If 'index' is bigger than the number of sensors in the list, the list node is inserted at the end of the list.
 * - If 'list' is is NULL, NULL is returned.
 * \param list a pointer to the list
 * \param sensor a pointer to the data that needs to be inserted
 * \param index the position at which the sensor should be inserted in the list
 * \param insert_copy if true use sensor_copy() to make a copy of 'sensor' and use the copy in the new list node, otherwise the given sensor pointer is added to the list
 * \return a pointer to the list or NULL
 */
dplist_t *dpl_insert_at_index(dplist_t *list, void *sensor, int index, bool insert_copy);

/** Removes the list node at index 'index' from the list.
 * - The list node itself should always be freed.
 * - If 'index' is 0 or negative, the first list node is removed.
 * - If 'index' is bigger than the number of sensors in the list, the last list node is removed.
 * - If the list is empty, return the unmodified list.
 * - If 'list' is is NULL, NULL is returned.
 * \param list a pointer to the list
 * \param index the position at which the node should be removed from the list
 * \param free_sensor if true, call sensor_free() on the sensor of the list node to remove
 * \return a pointer to the list or NULL
 */
dplist_t *dpl_remove_at_index(dplist_t *list, int index, bool free_sensor);

/** Returns a reference to the list node with index 'index' in the list.
 * - If 'index' is 0 or negative, a reference to the first list node is returned.
 * - If 'index' is bigger than the number of list nodes in the list, a reference to the last list node is returned.
 * - If the list is empty, NULL is returned.
 * - If 'list' is is NULL, NULL is returned.
 * \param list a pointer to the list
 * \param index the position of the node for which the reference is returned
 * \return a pointer to the list node at the given index or NULL
 */
dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index);

/** Returns the list sensor contained in the list node with index 'index' in the list.
 * - return is not returning a copy of the sensor with index 'index', i.e. 'sensor_copy()' is not used.
 * - If 'index' is 0 or negative, the sensor of the first list node is returned.
 * - If 'index' is bigger than the number of sensors in the list, the sensor of the last list node is returned.
 * - If the list is empty, NULL is returned.
 * - If 'list' is NULL, NULL is returned.
 * \param list a pointer to the list
 * \param index the position of the node for which the sensor is returned
 * \return a pointer to the sensor at the given index or NULL
 */
void *dpl_get_sensor_at_index(dplist_t *list, int index);

/** Returns an index to the first list node in the list containing 'sensor'.
 * - the first list node has index 0.
 * - Use 'sensor_compare()' to search 'sensor' in the list, a match is found when 'sensor_compare()' returns 0.
 * - If 'sensor' is not found in the list, -1 is returned.
 * - If 'list' is NULL, NULL is returned.
 * \param list a pointer to the list
 * \param sensor the sensor to look for
 * \return the index of the sensor that matches 'sensor'
 */
int dpl_get_index_of_sensor(dplist_t *list, void *sensor);

/** Returns the sensor contained in the list node with reference 'reference' in the list.
 * - If the list is empty, NULL is returned.
 * - If 'list' is is NULL, NULL is returned.
 * - If 'reference' is NULL, NULL is returned.
 * - If 'reference' is not an existing reference in the list, NULL is returned.
 * \param list a pointer to the list
 * \param reference a pointer to a certain node in the list
 * \return the sensor contained in the list node or NULL
 */
void *dpl_get_sensor_at_reference(dplist_t *list, dplist_node_t *reference);

#endif  // _DPLIST_H_

