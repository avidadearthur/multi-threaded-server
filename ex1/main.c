#include <stdio.h>
#include "sensor_db.h"
#include <time.h>

int main(){
    printf("plab2sensordb ex1 main\n");

    // input
    FILE * db;
    db = open_db("sensor_db.txt", true);

    // num of sec since 01/01/70
    time_t rawtime;
    time ( &rawtime );

    insert_sensor(db,2,11.11,time ( &rawtime ));
    insert_sensor(db,3,15.13,time ( &rawtime ));

    close_db(db);

    // output
    //
    // https://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c

    FILE* file = fopen("sensor_db.txt", "r"); /* should check the result */
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s", line);
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);

    return 0;
}