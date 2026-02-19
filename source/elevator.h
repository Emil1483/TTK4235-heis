#pragma once
#include "driver/elevio.h"
#include "hardwareReader.h"
#include <stdio.h>
#include <stdlib.h>

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
    void (*update_floor)(struct Elevator* elevator, int floor);
    void (*set_stopped)(struct Elevator* elevator, int shouldStop);
    void (*set_obstructed)(struct Elevator* elevator, int obstructed);
    void (*order)(struct Elevator* elevator, int floor, ButtonType button);
};

void update_floor(struct Elevator* elevator, int floor);
void set_stopped(struct Elevator* elevator, int shouldStop);
void set_obstructed(struct Elevator* elevator, int obstructed);
void order(struct Elevator* elevator, int floor, ButtonType button);

struct Elevator* elevator_constructor(int floor);