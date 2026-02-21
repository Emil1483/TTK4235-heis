#include "driver/elevio.h"
#include "elevator.h"
#include "hardwareReader.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    elevio_init();

    struct HardwareReader *reader = hardwareReader_constructor();

    printf("=== Djupvik et. al. ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    while (1) {
        reader->doReading(reader);
        nanosleep(&(struct timespec){0, 20 * 1000 * 1000}, NULL);
    }

    return 0;
}
