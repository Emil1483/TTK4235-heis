#include "driver/elevio.h"
#include "hardwareReader.h"
#include <stdio.h>

struct HardwareReader hardwareReader_constructor()
{
    int floor = elevio_floorSensor();
    struct Elevator elevator = elevator_constructor(floor);
    struct HardwareReader hardwareReader = {&elevator, doReading};
    return hardwareReader;
}

void doReading(struct HardwareReader* reader)
{
    int floor = elevio_floorSensor();
    switch (floor)
    {
    case -1:
        break;
    default:
        reader->elevator->update_floor(reader->elevator, floor);
        break;
    }
    printf("\nFloor: %d\n", floor); //debug

    int shouldStop = elevio_stopButton(); //debug
    printf("shouldStop: %d\n", shouldStop);
    reader->elevator->set_stopped(reader->elevator,shouldStop);

    int obstructed = elevio_obstruction();
    reader->elevator->set_obstructed(reader->elevator, obstructed);

    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int button = 0; button < N_BUTTONS; button++)
        {
            int isPressed = elevio_callButton(floor, button);
            switch (isPressed)
            {
            case 1:
                // order
                printf("___________\nMake order (from hardwareReader):\nFloor: %d\nButton: %d", floor, button);
                reader->elevator->order(reader->elevator, floor, (ButtonType)button);
                break;
            default:
                break;
            }
        }
    }
}