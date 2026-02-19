#pragma once
#include "driver/elevio.h"
#include "hardwareReader.h"
#include <stdio.h>

enum State{
    GOING_UP, 
    DOORS_OPEN, 
    DOOR_CLOSED, 
    GOING_DOWN,
    UNKNOWN
};

struct Queue{
    int i;//make the queue
};

struct Elevator{
    enum State state;
    struct Queue queue;
    //Timer timer;
    int current_floor;
    int obstructed;
};

struct Elevator elevator_constructor(int floor);