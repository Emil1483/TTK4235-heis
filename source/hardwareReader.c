#include "driver/elevio.h"
#include "hardwareReader.h"
#include <stdio.h>

struct HardwareReader hardwareReader_constructor(){
    int floor = elevio_floorSensor();
    struct Elevator elevator=elevator_constructor(floor);
    struct HardwareReader hardwareReader={elevator, doReading};
    return hardwareReader;
}

void doReading(){
    int floor = elevio_floorSensor();
    //int floor=1;
    switch (floor){
        case 1:
            //update floor
            break;
        case 2:
            //update floor
            break;
        case 3:
            //update floor
            break;
        case 4:
            //update floor
            break;
        default:
            break;
    }
    printf("\nFloor: %d\n",floor);
    
    int shouldStop=elevio_stopButton();
    printf("shouldStop: %d\n",shouldStop);
    //setStopped
    
    int obstructed=elevio_obstruction();
    //set obstructed
    
    for(int floor=0;floor<N_FLOORS;floor++){
        for(int button=0;button<N_BUTTONS;button++){
            int isPressed=elevio_callButton(floor, button);
            switch (isPressed){
                case 1:
                    //order
                    printf("Make order: \n")
                    break;
                default:
                    break;
            }
        }
    }
    
    
}