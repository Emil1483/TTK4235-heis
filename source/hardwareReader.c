#include "hardwareReader.h"
#include "driver/elevio.h"
#include <stdio.h>

struct HardwareReader *hardwareReader_constructor() {
    struct HardwareReader *reader = malloc(sizeof(struct HardwareReader));
    if (!reader) {
        perror("malloc failed to make HardwareReader!\n");
        exit(1);
    }
    int floor = elevio_floorSensor();
    reader->elevator = elevator_constructor(floor);
    reader->doReading = doReading;
    return reader;
}

void doReading(struct HardwareReader *reader) {
    int floor = elevio_floorSensor();
    switch (floor) {
    case -1:
        break;
    default:
        update_floor(reader->elevator, floor);
        break;
    }
    int shouldStop = elevio_stopButton();
    set_stopped(reader->elevator, shouldStop);

    int obstructed = elevio_obstruction();
    set_obstructed(reader->elevator, obstructed);

    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            int isPressed = elevio_callButton(floor, button);
            switch (isPressed) {
            case 1:
                order(reader->elevator, floor, (ButtonType)button);
                break;
            default:
                break;
            }
        }
    }
}