/**
 * \author Arthur Tavares Quintao
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>

/**
 * Operation to open a text file with a given name, and providing an indication
 * if the file should be overwritten if the file already exists or if the data should
 * be appended to the existing file.
 *
 * \param filename : 'sensor_db.txt'
 * \param append : overwrite or not
 * \return FILE pointer
 */
FILE * open_db(char * filename, bool append);

/**
 * Operation to append a single sensor reading to the csv file.
 *
 * File format example:
 *
 *       sensorId    sensorValue     timestamp
 *   l0       1           12           12:00:00
 *   l1       2           10           12:01:34
 *      ...      ...         ...             ...
 *
 * \param f : FILE pointer
 * \param id : sensor id
 * \param value : sensor value
 * \param ts : sensor timestamp
 * \return error code
 */
int insert_sensor(FILE * f, sensor_id_t id, sensor_value_t value, sensor_ts_t ts);

/**
 * Operation to close the csv file.
 *
 * \param f : FILE pointer
 * \return error code
 */
int close_db(FILE * f);