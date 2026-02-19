#include "driver/elevio.h"
#include "hardwareReader.h"
#include "elevator.h"


struct Elevator elevator_constructor(int floor){
    struct Elevator elevator;
    struct Queue queue={1};
    elevator.queue=queue;
    switch (floor){
        case -1:
            elevator.state=UNKNOWN;
        default:
            elevator.state=DOOR_CLOSED;
    };
    elevator.current_floor=floor;
    elevator.obstructed=0;
    return elevator;
}
