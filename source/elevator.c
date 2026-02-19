#include "driver/elevio.h"
#include "hardwareReader.h"
#include "elevator.h"

void update_floor(struct Elevator* elevator, int floor){
    printf("Floor was not updated, function not implemented yet!\n");
}
void set_stopped(struct Elevator* elevator, int shouldStop){
    printf("stopped was not updated, function not implemented yet!\n");
}
void set_obstructed(struct Elevator* elevator, int obstructed){
    printf("obstructed not set, function not implemented yet!\n");
}
void order(struct Elevator* elevator, int floor, ButtonType button){
    printf("No order set, function not implemented yet!\n");
}


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
