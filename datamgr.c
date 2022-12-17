/**
 * \author Arthur Tavares Quintao
 */
#include "lib/dplist.h"
#include "datamgr.h"
#include "config.h"
#include "sbuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

dplist_t *sensorList;
extern sbuffer_t *shared_buffer;

//--------------------------dplist_t *lastValues-----------------------------//
void previousValue_copy(void * previousValue);
void previousValue_free(void ** previousValue);
int previousValue_compare(void * x, void * y);

void previousValue_copy(void * previousValue){};
void previousValue_free(void ** previousValue){};
int previousValue_compare(void * x, void * y){
    return -1;
};
//--------------------------dplist_t *lastValues-----------------------------//

//---------------------------plist_t *sensorList----------------------------//
struct sensor_element {

    sensor_id_t sensorId;
    int roomId;
    double  avg;
    sensor_ts_t lastModified;
    dplist_t *lastValues; // dplist that has sensor_data_t as sensors

} ;

void sensor_copy(void * sensor);
void sensor_free(void ** sensor);
int sensor_compare(void * x, void * y);

void sensor_copy(void * sensor){};
void sensor_free(void ** sensor) {
    dpl_free(&((sensor_t *)*sensor)->lastValues, true);
    free(*sensor);
    *sensor = NULL;
}
int sensor_compare(void * x, void * y) {
    return ((((sensor_t*)x)->sensorId < ((sensor_t*)y)->sensorId) ? -1 : (((sensor_t*)x)->sensorId == ((sensor_t*)y)->sensorId) ? 0 : 1);
}
//-------------------------plist_t *sensorList---------------------------------//

/**
 * The reader threads repeat reading a complete
 * sensor measurement from the shared buffer and
 * write this data to a common file called ‘sensor_data_out’.
 *
 * @param arg The file pointer to the sensor_data file
 * @return NULL
 * */
void *data_manager(void *fp)
{
    char *message;

    FILE * map = fopen("room_sensor.map", "r");

    if (!map)perror("fopen");

    datamgr_parse_sensor_files(map);
    fclose(map);

    // Init data as sensor_data_t
    sensor_data_t data;
    do{
        // read data from buffer
        if (sbuffer_peek(shared_buffer, &data) == 0) { // SBUFFER_SUCCESS 0

            // check the sensorId of data to find the corresponding sensor in sensorList
            sensor_t *sensor = datamgr_get_sensor_by_id(data.id);
            if (sensor == NULL) {
                // ----------Received sensor data with invalid sensor node ID Event-----------//
                message = "Received sensor data with invalid sensor node ID: %d";
                write_to_pipe(message, data.id);
                // --------------------------------------------------------------------------//
                continue;
            }
            // else, fill values
            // For a RUN_AVG_LENGTH of 5, the average is calculated as follows:
            // check if the dplist sensor->lastValues has size smaller than 5
            if (dpl_size(sensor->lastValues) < RUN_AVG_LENGTH) {
                // append data to sensor->lastValues
                dpl_insert_at_index(sensor->lastValues, &data, dpl_size(sensor->lastValues), true);
            } else {
                // remove the first element in sensor->lastValues
                dpl_remove_at_index(sensor->lastValues, 0, true);
                // append data to sensor->lastValues
                dpl_insert_at_index(sensor->lastValues, &data, dpl_size(sensor->lastValues), true);
            }
            // calculate the average from the lastValues dplist
            sensor->avg = 0;
            for (int i = 0; i < dpl_size(sensor->lastValues); i++) {
                sensor_data_t *sensor_data = dpl_get_element_at_index(sensor->lastValues, i);
                sensor->avg += sensor_data->value;
            }
            sensor->avg = sensor->avg / dpl_size(sensor->lastValues);
            // update the lastModified
            sensor->lastModified = data.ts;

            // check if temp is too high or too low
            if (data.value > SET_MAX_TEMP) {
                // ----------Temperature too high Event-----------//
                message = malloc(sizeof(char) * 100);
                sprintf(message, "Sensor node %d reports it’s too hot (avg temp = %f)", sensor->sensorId, sensor->avg);
                message = realloc(message, strlen(message)+1); // trim the string
                write_to_pipe(message, -1);
                // ----------------------------------------------//
            } else if (data.value < SET_MIN_TEMP) {
                // ----------Temperature too low Event-----------//
                message = malloc(sizeof(char) * 100);
                sprintf(message, "Sensor node %d reports it’s too cold (avg temp = %f)", sensor->sensorId, sensor->avg);
                message = realloc(message, strlen(message)+1); // trim the string
                write_to_pipe(message, -1);
                // ---------------------------------------------//
            }

            // ----------Received sensor data with valid sensor node ID Event-----------//
/*          message = malloc(sizeof(char) * 100);
            struct tm *tsm = gmtime(&(sensor->lastModified));
            char timestamp[256];
            strftime(timestamp, sizeof(timestamp), "%F %T", tsm);
            sprintf(message, "Sensor %d, average of last %d samples: %.2f, at %s", sensor->sensorId,RUN_AVG_LENGTH, sensor->avg, timestamp);
            message = realloc(message, strlen(message)+1); // trim the string
            write_to_pipe(message, data.id);*/
            // --------------------------------------------------------------------------//
        }
    }
    while(data.id != 0);

    return NULL;
}

//------------------------datamgr.h Implementations----------------------------//
void datamgr_parse_sensor_files(FILE *fp_sensor_map){

    //----------------------- Parsing .map file ------------------------//
    char    *text;
    char    *buff;
    long    numbytes;

    fseek(fp_sensor_map, 0L, SEEK_END);
    numbytes = ftell(fp_sensor_map);
    fseek(fp_sensor_map, 0L, SEEK_SET);

    text = (char*)calloc(numbytes, sizeof(char));
    if(text == NULL){ printf("Failed to parse line: text is NULL");}
    fread(text, sizeof(char), numbytes, fp_sensor_map);

    sensorList = dpl_create(NULL, sensor_free, sensor_compare);

    int i = 0;
    sensor_t *sensor;
    buff = strtok(text," \n");
    while (buff!= NULL)
    {
        if(i == 0){
            sensor = (sensor_t *)malloc(sizeof(sensor_t)); // create a new sensor
            sensor->roomId = atoi(buff);
            buff = strtok(NULL, " \n");
            i++;
        }
        else{
            sensor->sensorId = atoi(buff);
            buff = strtok(NULL, " \n");
            i = 0;
            sensor->avg = 0.0;
            sensor->lastValues = dpl_create(NULL, previousValue_free, previousValue_compare);
            sensor->lastModified = 0;
            // add sensor to sensorList
            dpl_insert_at_index(sensorList, sensor, dpl_size(sensorList), false);
        }
    }
    free(text);
}

void datamgr_free(){
    dpl_free(&sensorList, true);
}

sensor_t *datamgr_get_sensor_by_id(sensor_id_t sensor_id){
    sensor_t *sensor;
    int size = dpl_size(sensorList);
    for(int i = 0; i < size; i++){
        sensor = (sensor_t *)dpl_get_element_at_index(sensorList, i);
        if(sensor->sensorId == sensor_id){
            return sensor;
        }
    }
    return NULL;
}

uint16_t datamgr_get_room_id(sensor_id_t sensor_id){
    sensor_t *sensor;
    sensor = datamgr_get_sensor_by_id(sensor_id);
    if(sensor != NULL){
        return sensor->roomId;
    }
    return -1;
}

sensor_value_t datamgr_get_avg(sensor_id_t sensor_id){
    sensor_t *sensor;
    sensor = datamgr_get_sensor_by_id(sensor_id);
    if(sensor != NULL){
        return sensor->avg;
    }
    return -1;
}

time_t datamgr_get_last_modified(sensor_id_t sensor_id){
    sensor_t *sensor;
    sensor = datamgr_get_sensor_by_id(sensor_id);
    if(sensor != NULL){
        return sensor->lastModified;
    }
    return -1;
}

int datamgr_get_total_sensors(){
    return dpl_size(sensorList);
}

sensor_t * datamgr_get_sensor(sensor_id_t sensor_id){
    sensor_t *sensor = (sensor_t *)malloc(sizeof(sensor_t));
    sensor->sensorId = sensor_id;
    return dpl_get_element_at_index(sensorList, dpl_get_index_of_element(sensorList, sensor));
}

dplist_t * datamgr_get_sensors(){
    return sensorList;
}