#pragma once
#include "driver/elevio.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "orderMatrix.h"

typedef enum
{
    UNKNOWN,
    DOORS_CLOSED,
    GOING_DOWN,
    STOPPED,
    BETWEEN_FLOORS,
    GOING_UP,
    DOORS_OPEN,
} State;

typedef struct
{
    State state;
    State last_mving_state;
    OrderMatrix *order_matrix;
    Timer *timer;
    int current_floor;
    bool obstructed;
} Elevator;

void update_floor(Elevator *elevator, int floor);
void set_stopped(Elevator *elevator, int shouldStop);
void set_obstructed(Elevator *elevator, int obstructed);
void order(Elevator *elevator, int floor, ButtonType button);
bool should_stop_at_floor(Elevator *elevator);
State state_after_completed_order(Elevator *elevator);
void update_state(Elevator *p_elevator, State state);

Elevator *elevator_constructor(int floor);