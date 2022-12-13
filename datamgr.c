#include "lib/dplist.h"
#include "datamgr.h"
#include "config.h"
#include "sbuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

dplist_t *sensorList;
extern sbuffer_t *buffer;

//--------------------------dplist_t *lastValues-----------------------------//

// !!!
// when comes the time to add values to *lastValues you can add double sensor_value_t;
// by mallocing a pointer to a double and then assigning the data->value to it

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
    //-----------------------------------plab1-----------------------------------------//

    FILE * map = fopen("room_sensor.map", "r");

    if (!map)perror("fopen");

    datamgr_parse_sensor_files(map);

    // check if the sensorList is created
    dplist_t *sensorList = datamgr_get_sensors();
    assert(sensorList != NULL);
    printf("sensorList is created\n");

    // check the size of the sensorList
    assert(dpl_size(sensorList) == datamgr_get_total_sensors());
    printf("sensorList size: %d\n", dpl_size(sensorList));
    printf("sensorList size: %d\n", datamgr_get_total_sensors());

    // check if sensorId and roomId are correct based on expected.txt
    sensor_id_t id;
    sensor_t *sensor0 = dpl_get_element_at_index(sensorList, 0);
    id = sensor0->sensorId;
    assert(datamgr_get_room_id(id)==1);
    sensor_t *sensor1 = dpl_get_element_at_index(sensorList, 1);
    id = sensor1->sensorId;
    assert(datamgr_get_room_id(id)==2);
    sensor_t *sensor2 = dpl_get_element_at_index(sensorList, 2);
    id = sensor2->sensorId;
    assert(datamgr_get_room_id(id)==3);
    sensor_t *sensor3 = dpl_get_element_at_index(sensorList, 3);
    id = sensor3->sensorId;
    assert(datamgr_get_room_id(id)==4);
    sensor_t *sensor4 = dpl_get_element_at_index(sensorList, 4);
    id = sensor4->sensorId;
    assert(datamgr_get_room_id(id)==11);
    sensor_t *sensor5 = dpl_get_element_at_index(sensorList, 5);
    id = sensor5->sensorId;
    assert(datamgr_get_room_id(id)==12);
    sensor_t *sensor6 = dpl_get_element_at_index(sensorList, 6);
    id = sensor6->sensorId;
    assert(datamgr_get_room_id(id)==13);
    sensor_t *sensor7 = dpl_get_element_at_index(sensorList, 7);
    id = sensor7->sensorId;
    assert(datamgr_get_room_id(id)==14);
    printf("SensorId and RoomId are correct\n");

    datamgr_free();

    fclose(map);

    //-----------------------------------plab1-----------------------------------------//

    // Init data as sensor_data_t
    sensor_data_t data;
    do{
        // read data from buffer
        if (sbuffer_remove(buffer, &data) == 0) { // SBUFFER_SUCCESS 0

            // check the sensorId of data to find the corresponding sensor in sensorList
            sensor_t *sensor = datamgr_get_sensor_by_id(data.id);
            if (sensor == NULL) {
                printf("Sensor is not present in map file.\n");
                continue;
            }
            // else, fill values







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

    //------------ Start t update the dplist based on fp_sensor_data -------------//

    // In the final project sensor_data is no longer a file but the shared buffer.
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

//----------------------------datamgr.h Implementations--------------------------------------//
