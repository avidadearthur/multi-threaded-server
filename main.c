#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "sbuffer.h"

sbuffer_t *buffer;

void *writer(void *arg);
void *reader(void *arg);

int main()
{
    // Initialize the buffer
    sbuffer_init(&buffer);

    // Open sensor_data file
    FILE *sensor_data_fp;
    sensor_data_fp = fopen("sensor_data", "rb");

    // Start thread for reading sensor_data file adding elements to the sbuffer
    pthread_t writer_thread;
    pthread_create(&writer_thread, NULL, &writer, sensor_data_fp);

    // Open sensor_data_out file
    FILE *sensor_data_out_fp;
    sensor_data_out_fp = fopen("sensor_data_out", "w");

    // Start thread 1 and 2 for writing sensor_data_out file
    pthread_t reader_thread1;
    pthread_t reader_thread2;
    pthread_create(&reader_thread1, NULL, &reader, sensor_data_out_fp);
    pthread_create(&reader_thread2, NULL, &reader, sensor_data_out_fp);

    // Wait for threads to finish and join them
    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread1, NULL);
    pthread_join(reader_thread2, NULL);

    // Close sensor_data file
    fclose(sensor_data_fp);
    // Close sensor_data_out file
    //fclose(sensor_data_out_fp);

    // free buffer
    sbuffer_free(&buffer);

    return 0;
}

/**
 * The writer thread reads sensor data from a single file (ID, temperature and timestamp)
 * called ‘sensor_data’ and inserts this sensor data in the shared buffer.
 *
 * @param arg The file pointer to the sensor_data file
 * @return NULL
 * */
void *writer(void *fp)
{
    // cast fp to FILE
    FILE *sensor_data_fp = (FILE *)fp;

    // make char buffers of size sensor_id_t, sensor_value_t and sensor_ts_t
    char sensor_id_buffer[sizeof(sensor_id_t)];
    char sensor_value_buffer[sizeof(sensor_value_t)];
    char sensor_ts_buffer[sizeof(sensor_ts_t)];

    // parse sensor_data file into sensor_id_buffer, sensor_value_buffer and sensor_ts_buffer
    while(fread(sensor_id_buffer, sizeof(sensor_id_t), 1, sensor_data_fp) == 1 &&
    fread(sensor_value_buffer, sizeof(sensor_value_t), 1, sensor_data_fp) == 1 &&
    fread(sensor_ts_buffer, sizeof(sensor_ts_t), 1, sensor_data_fp)) {
        // create sensor_data_t
        sensor_data_t sensor_data;
        // copy sensor_id_buffer to sensor_data.id
        memcpy(&sensor_data.id, sensor_id_buffer, sizeof(sensor_id_t));
        // copy sensor_value_buffer to sensor_data.value
        memcpy(&sensor_data.value, sensor_value_buffer, sizeof(sensor_value_t));
        // copy sensor_ts_buffer to sensor_data.ts
        memcpy(&sensor_data.ts, sensor_ts_buffer, sizeof(sensor_ts_t));

        // print sensor_data for testing
        // printf("sensor_data.id: %d, sensor_data.value: %f, sensor_data.ts: %ld\n", sensor_data.id, sensor_data.value, sensor_data.ts);

        // insert sensor_data into buffer
        sbuffer_insert(buffer, &sensor_data);
    }
    // Add dummy data to buffer to signal end of file
    sensor_data_t sensor_data;
    sensor_data.id = 0;
    sensor_data.value = 0;
    sensor_data.ts = 0;
    sbuffer_insert(buffer, &sensor_data);

    return NULL;
}

/**
 * The reader threads repeat reading a complete
 * sensor measurement from the shared buffer and
 * write this data to a common file called ‘sensor_data_out’.
 *
 * @param arg The file pointer to the sensor_data file
 * @return NULL
 * */
void *reader(void *fp)
{
    // cast fp to FILE
    //FILE *sensor_data_out_fp = (FILE *)fp;

    // Init data as sensor_data_t
    sensor_data_t data;
    do{
        // read data from buffer
        sbuffer_remove(buffer, &data);
        // write data to sensor_data_out file
        // fwrite(&data, sizeof(sensor_data_t), 1, sensor_data_out_fp);
        // print data for testing
        printf("data.id: %d, data.value: %f, data.ts: %ld \n", data.id, data.value, data.ts);
    }
    while(data.id != 0);

    // free allocated memory
    // free(fp);

    return NULL;
}
