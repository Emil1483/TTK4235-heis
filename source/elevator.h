#pragma once
#include "driver/elevio.h"
#include "queue.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    UNKNOWN,
    DOORS_CLOSED,
    GOING_DOWN,
    STOPPED,
    BETWEEN_FLOORS,
    GOING_UP,
    DOORS_OPEN,
} State;

typedef struct {
    State state;
    Queue *queue;
    Timer *timer;
    int current_floor;
    int obstructed;
} Elevator;

void update_floor(Elevator *elevator, int floor);
void set_stopped(Elevator *elevator, int shouldStop);
void set_obstructed(Elevator *elevator, int obstructed);
void order(Elevator *elevator, int floor, ButtonType button);

Elevator *elevator_constructor(int floor);