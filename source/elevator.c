#include "elevator.h"
#include "driver/elevio.h"
#include "hardwareReader.h"
#include "queue.h"

#define TIMER_DELAY 3.0

void update_floor(Elevator *elevator, int floor) {
    if (elevator->state == UNKNOWN) {
        elevator->state = DOORS_CLOSED;
        elevio_motorDirection(DIRN_STOP);
        elevator->current_floor = floor;
        return;
    }

    if (elevator->current_floor == floor) {
        return;
    }

    elevator->current_floor = floor;

    if (elevator->queue->length == 0 ||
        get(elevator->queue, 0).floor_number != floor) {
        return;
    }

    remove_orders(elevator->queue, floor);
    elevator->state = DOORS_OPEN;
    elevio_motorDirection(DIRN_STOP);
    reset(elevator->timer, TIMER_DELAY);
}
void set_stopped(Elevator *elevator, int shouldStop) {}
void set_obstructed(Elevator *elevator, int obstructed) {
    elevator->obstructed = obstructed;
}
void order(Elevator *elevator, int floor, ButtonType button) {
    Order order;
    order.button_type = button;
    order.floor_number = floor;
    place_order(elevator->queue, elevator->queue->length, order);
    if (elevator->queue->length == 1) {
        // The queue was empty
        switch (elevator->state) {
        case DOORS_CLOSED:
            if (floor > elevator->current_floor) {
                elevator->state = DOORS_OPEN;
                remove_orders(elevator->queue, floor);
            } else if (floor > elevator->current_floor) {
                elevator->state = GOING_UP;
                elevio_motorDirection(DIRN_UP);
            } else {
                elevator->state = GOING_DOWN;
                elevio_motorDirection(DIRN_DOWN);
            }
            break;
        case BETWEEN_FLOORS:
            if (floor > elevator->current_floor) {
                elevator->state = GOING_UP;
                elevio_motorDirection(DIRN_UP);
            } else {
                elevator->state = GOING_DOWN;
                elevio_motorDirection(DIRN_DOWN);
            }
            break;
        }
    }
}
void on_timer_fire(void *arg) {
    Elevator *p_elevator = (Elevator *)arg;
    switch (p_elevator->state) {
    case DOORS_CLOSED:
        if (p_elevator->queue->length == 0) {
            p_elevator->state = DOORS_OPEN;
            break;
        } else {
            Order order = get(p_elevator->queue, 0);
            if (order.floor_number > p_elevator->current_floor) {
                p_elevator->state = GOING_UP;
                elevio_motorDirection(DIRN_UP);
            } else {
                p_elevator->state = GOING_DOWN;
                elevio_motorDirection(DIRN_DOWN);
            }
            break;
        }
    case DOORS_OPEN:
        if (p_elevator->queue->length == 0) {
            p_elevator->state = DOORS_CLOSED;
            break;
        } else {
            Order order = get(p_elevator->queue, 0);
            if (order.floor_number > p_elevator->current_floor) {
                p_elevator->state = GOING_UP;
                elevio_motorDirection(DIRN_UP);
            } else {
                p_elevator->state = GOING_DOWN;
                elevio_motorDirection(DIRN_DOWN);
            }
            break;
        }
    }
}

Elevator *elevator_constructor(int floor) {
    Elevator *elevator = malloc(sizeof(Elevator));
    if (!elevator) {
        perror("malloc failed to make Elevator!\n");
        exit(1);
    }
    elevator->queue = init_queue();
    elevator->timer = init_timer(elevator, on_timer_fire);
    switch (floor) {
    case -1:
        elevator->state = UNKNOWN;
        elevio_motorDirection(DIRN_UP);
        break;
    default:
        elevator->state = DOORS_CLOSED;
        break;
    };
    elevator->current_floor = floor;
    elevator->obstructed = 0;
    return elevator;
}
