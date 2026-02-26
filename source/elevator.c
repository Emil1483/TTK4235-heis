#include "elevator.h"

#define TIMER_DELAY 3.0

void update_floor(Elevator *elevator, int floor)
{
    // Remember that this function is only called when the
    // sensor is active

    if (elevator->current_floor != floor)
    {
        printf("current floor is now %d\n", floor);
        elevio_floorIndicator(floor);
    }

    elevator->current_floor = floor;

    if (elevator->state == UNKNOWN)
    {
        update_state(elevator, DOORS_CLOSED);
        return;
    }

    if (should_stop_at_floor(elevator))
    {
        update_state(elevator, DOORS_OPEN);
    }
}

void set_stopped(Elevator *elevator, int shouldStop)
{
    // ref FAT spec O3
    if (elevator->state == UNKNOWN)
        return;

    // Only do somethig if we're switching to stop or from stop
    if (shouldStop && elevator->state == STOPPED)
        return;
    if (!shouldStop && elevator->state != STOPPED)
        return;

    // while in the stopped state, we released the stop button
    if (elevator->state == STOPPED)
    {
        update_state(elevator, BETWEEN_FLOORS);
        return;
    }

    // while not in the stopped state, we pressed the stop button
    if (elevator->state == GOING_DOWN || elevator->state == GOING_UP || elevator->state == BETWEEN_FLOORS)
    {
        // if we're moving, set state to stopped
        update_state(elevator, STOPPED);
    }
    else
    {
        // if not moving, open doors, clear matrix, reset timer closes the door after a timeout
        update_state(elevator, DOORS_OPEN);
        clear_matrix(elevator->order_matrix);
        reset(elevator->timer, TIMER_DELAY);
    }
}

void set_obstructed(Elevator *elevator, int obstructed)
{
    elevator->obstructed = obstructed;
    if (obstructed && elevator->state == DOORS_OPEN)
        reset(elevator->timer, TIMER_DELAY);
}

void on_timer_fire(void *arg)
{
    Elevator *p_elevator = (Elevator *)arg;

    State newstate = state_after_completed_order(p_elevator);
    update_state(p_elevator, newstate);
}

void order(Elevator *elevator, int floor, ButtonType button)
{

    if (elevator->state == STOPPED || elevator->state == UNKNOWN)
        return;

    if (elevator->state == DOORS_OPEN && elevator->current_floor == floor)
    {
        reset(elevator->timer, TIMER_DELAY);
        return;
    }

    if (elevator->state == DOORS_CLOSED && elevator->current_floor == floor)
    {
        update_state(elevator, DOORS_OPEN);
        return;
    }

    if (matrix_is_empty(elevator->order_matrix) && !elevator->timer->waiting)
    {
        if (floor == elevator->current_floor && elevator->state != BETWEEN_FLOORS)
        {
            update_state(elevator, DOORS_OPEN);
        }
        else
        {
            elevator->order_matrix->matrix[floor][button] = 1;
            elevio_buttonLamp(floor, button, 1);
            if (floor > elevator->current_floor)
            {
                update_state(elevator, GOING_UP);
            }
            else if (floor < elevator->current_floor)
            {
                update_state(elevator, GOING_DOWN);
            }
            else
            {
                if (elevator->last_mving_state == GOING_DOWN)
                    update_state(elevator, GOING_UP);
                else
                    update_state(elevator, GOING_DOWN);
            }
        }
    }
    else
    {
        elevator->order_matrix->matrix[floor][button] = 1;
        elevio_buttonLamp(floor, button, 1);
    }

    printf("we just ordered. Now the matrix is\n");
    print_matrix(elevator->order_matrix);
}

bool should_stop_at_floor(Elevator *elevator)
{
    switch (elevator->state)
    {
    case GOING_DOWN:
        if (check_if_order_at_floor(elevator->order_matrix, elevator->current_floor, BUTTON_HALL_UP))
            return true;
        return !check_if_order_below(elevator->order_matrix, elevator->current_floor - 1);
    case GOING_UP:
        if (check_if_order_at_floor(elevator->order_matrix, elevator->current_floor, BUTTON_HALL_DOWN))
            return true;
        return !check_if_order_above(elevator->order_matrix, elevator->current_floor + 1);
    default:
        // if we stop between floors and try to get back to the previous floor (current_floor), we miss it because
        // we only do stuff in update_floor if floor is changed.
        // The fix is to do stuff no matter what
        return false;
    }
}

State state_after_completed_order(Elevator *elevator)
{
    switch (elevator->last_mving_state)
    {
    case GOING_DOWN:
        if (check_if_order_below(elevator->order_matrix, elevator->current_floor - 1))
            return GOING_DOWN;
        if (check_if_order_above(elevator->order_matrix, elevator->current_floor))
            return GOING_UP;
        break;
    default:
        if (check_if_order_above(elevator->order_matrix, elevator->current_floor + 1))
            return GOING_UP;
        if (check_if_order_below(elevator->order_matrix, elevator->current_floor))
            return GOING_DOWN;
        break;
    }
    return DOORS_CLOSED;
}

Elevator *elevator_constructor(int floor)
{
    Elevator *elevator = malloc(sizeof(Elevator));
    if (!elevator)
    {
        perror("malloc failed to make Elevator!\n");
        exit(1);
    }
    elevator->order_matrix = init_matrix();
    clear_matrix(elevator->order_matrix); // for clearing all lights
    elevio_doorOpenLamp(0);
    elevio_stopLamp(0);
    elevator->last_mving_state = DOORS_CLOSED;
    elevator->timer = init_timer(elevator, on_timer_fire);
    switch (floor)
    {
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

void update_state(Elevator *p_elevator, State state)
{
    p_elevator->state = state;

    switch (state)
    {
    case DOORS_CLOSED:
        printf("new state: DOORS_CLOSED\n");
        elevio_motorDirection(DIRN_STOP);
        elevio_doorOpenLamp(0);
        elevio_stopLamp(0);
        break;

    case GOING_DOWN:
        p_elevator->last_mving_state = state;
        printf("new state: GOING_DOWN\n");
        elevio_motorDirection(DIRN_DOWN);
        elevio_doorOpenLamp(0);
        elevio_stopLamp(0);
        break;

    case STOPPED:
        printf("new state: STOPPED\n");
        elevio_motorDirection(DIRN_STOP);
        elevio_doorOpenLamp(0);
        elevio_stopLamp(1);
        clear_matrix(p_elevator->order_matrix);
        printf("new order matrix:\n");
        print_matrix(p_elevator->order_matrix);
        break;

    case BETWEEN_FLOORS:
        printf("new state: BETWEEN_FLOORS\n");
        elevio_motorDirection(DIRN_STOP);
        elevio_doorOpenLamp(0);
        elevio_stopLamp(0);
        break;

    case GOING_UP:
        p_elevator->last_mving_state = state;
        printf("new state: GOING_UP\n");
        elevio_motorDirection(DIRN_UP);
        elevio_doorOpenLamp(0);
        elevio_stopLamp(0);
        break;

    case DOORS_OPEN:
        printf("new state: DOORS_OPEN\n");
        elevio_motorDirection(DIRN_STOP);
        elevio_doorOpenLamp(1);
        elevio_stopLamp(0);
        reset(p_elevator->timer, TIMER_DELAY);
        for (int button = 0; button < N_BUTTONS; button++)
        {
            p_elevator->order_matrix->matrix[p_elevator->current_floor][button] = 0;
            elevio_buttonLamp(p_elevator->current_floor, button, 0);
        }
        break;

    default:
        printf("State change to %d not implemented\n", state);
        exit(1);
    }
}
