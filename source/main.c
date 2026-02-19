#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "hardwareReader.h"
#include "elevator.h"



int main(){
    elevio_init();
    
    struct HardwareReader* reader=hardwareReader_constructor();

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    //elevio_motorDirection(DIRN_UP);

    while(1){
        reader->doReading(reader);
    nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
