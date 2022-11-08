#include "lib/dplist.h"
#include "datamgr.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

dplist_t *sensorList;

typedef struct {

    sensor_id_t *sensorId;
    int roomId;
    sensor_value_t *sensorValue;
    double  avg;
    sensor_ts_t *timestamp;
    dplist_t *lastValues; // dplist that has sensor_data_t as sensors

} sensor_t;

void sensor_copy(void * sensor);
void sensor_free(void ** sensor);
int sensor_compare(void * x, void * y);

void sensor_copy(void * sensor){
    // do I need to implement this?
}

void sensor_free(void ** sensor) {

    free(((sensor_t *)*sensor)->sensorId);
    free(((sensor_t *)*sensor)->sensorValue);
    free(((sensor_t *)*sensor)->timestamp);

    dpl_free(&((sensor_t *)*sensor)->lastValues, true);

    free(*sensor);
    *sensor = NULL;
}

int sensor_compare(void * x, void * y) {
    return ((((sensor_t*)x)->sensorId < ((sensor_t*)y)->sensorId) ? -1 : (((sensor_t*)x)->sensorId == ((sensor_t*)y)->sensorId) ? 0 : 1);
}