#include "elevator.h"

#define TIMER_DELAY 3.0

void update_floor(Elevator *elevator, int floor) {
    if (elevator->state == UNKNOWN) {
        elevator->state = DOORS_CLOSED;
        elevio_motorDirection(DIRN_STOP);
        elevator->current_floor = floor;
        elevio_floorIndicator(floor);
        return;
    }
    if(elevator->state==BETWEEN_FLOORS || elevator->state==STOPPED) return;

    if(elevator->current_floor!=floor) elevio_floorIndicator(floor);
    elevator->current_floor = floor;

    if(!should_stop_at_floor(elevator)) return;

    for(int button=0;button<N_BUTTONS;button++){
        elevator->order_matrix->matrix[floor][button]=0;
        elevio_buttonLamp(floor, button, 0);
    }

    elevio_motorDirection(DIRN_STOP);
    elevator->state = DOORS_OPEN;
    elevio_doorOpenLamp(1);
    reset(elevator->timer, TIMER_DELAY);
}
void set_stopped(Elevator *elevator, int shouldStop) {
    if(shouldStop && elevator->state==STOPPED) return;
    if(!shouldStop && elevator->state!=STOPPED) return;
    if(elevator->state==STOPPED){
        elevator->state=BETWEEN_FLOORS;
        elevio_stopLamp(0);
        return;
    }
    if(elevator->state==GOING_DOWN || elevator->state==GOING_UP || elevator->state==BETWEEN_FLOORS){
        elevio_motorDirection(DIRN_STOP);
        elevator->state=STOPPED;
        clear_matrix(elevator->order_matrix);
        elevio_stopLamp(1);
    }
    else{
        elevio_doorOpenLamp(1);
        clear_matrix(elevator->order_matrix);
        elevator->state=DOORS_OPEN;
        reset(elevator->timer, TIMER_DELAY);
    }
}
void set_obstructed(Elevator *elevator, int obstructed) {
    elevator->obstructed = obstructed;
    if(obstructed && elevator->state==DOORS_OPEN) reset(elevator->timer,TIMER_DELAY);
}
void order(Elevator *elevator, int floor, ButtonType button) {
    if (elevator->state==STOPPED) return;
    elevator->order_matrix->matrix[floor][button]=1;
    elevio_buttonLamp(floor, button, 1);
    if(elevator->state == DOORS_CLOSED || elevator->state==BETWEEN_FLOORS){
        reset(elevator->timer,0); //cursed
    }
}
void on_timer_fire(void *arg) {
    Elevator *p_elevator = (Elevator *)arg;
    
    State newstate=state_after_completed_order(p_elevator);
    switch (newstate){
        case GOING_DOWN:
            p_elevator->last_mving_state=newstate;
            p_elevator->state=newstate;
            elevio_motorDirection(DIRN_DOWN);
            break;
        case GOING_UP:
            p_elevator->last_mving_state=newstate;
            p_elevator->state=newstate;
            elevio_motorDirection(DIRN_UP);
            break;
        default:
            p_elevator->state=newstate;
            break;
    }
    elevio_doorOpenLamp(0);
}
int should_stop_at_floor(Elevator *elevator){
    switch (elevator->state){
        case GOING_DOWN:
            for(int button=0;button<N_BUTTONS;button++){
                if (!(button==BUTTON_HALL_UP) && elevator->order_matrix->matrix[elevator->current_floor][button]){
                    return 1;
                }
            }
            for(int floor=elevator->current_floor-1;floor>=0;floor--){
                for(int button=0;button<N_BUTTONS;button++){
                    if(elevator->order_matrix->matrix[floor][button]) return 0;
                }
            }
            return 1;
            break;
        case GOING_UP:
            for(int button=0;button<N_BUTTONS;button++){
                if (!(button==BUTTON_HALL_DOWN) && elevator->order_matrix->matrix[elevator->current_floor][button]){
                    return 1;
                }
            }
            for(int floor=elevator->current_floor+1;floor<N_FLOORS;floor++){
                for(int button=0;button<N_BUTTONS;button++){
                    if(elevator->order_matrix->matrix[floor][button]) return 0;
                }
            }
            return 1;
            break;
        case DOORS_CLOSED:
            for(int button =0;button<N_BUTTONS;button++){
                if(elevator->order_matrix->matrix[elevator->current_floor][button]) return 1;
            }
            return 0;
        case DOORS_OPEN:
            for(int button =0;button<N_BUTTONS;button++){
                if(elevator->order_matrix->matrix[elevator->current_floor][button]) return 1;
            }
            return 0;
        default:
            printf("function 'should_stop_at_floor' was called from unexpected state %d\n",elevator->state);
            return 0;
            break;
    }
}

State state_after_completed_order(Elevator* elevator){
    switch (elevator->last_mving_state){
        case GOING_DOWN:
            for(int floor = elevator->current_floor-1;floor >= 0; floor--){
                for(int button=0;button<N_BUTTONS;button++){
                    if(elevator->order_matrix->matrix[floor][button]) return GOING_DOWN;
                }
            }
            for(int floor = elevator->current_floor;floor < N_FLOORS; floor++){
                for(int button=0;button<N_BUTTONS;button++){
                    if(elevator->order_matrix->matrix[floor][button]) return GOING_UP;
                }
            }
            break;
        default:
            for(int floor = elevator->current_floor+1;floor < N_FLOORS;floor++){
                for(int button=0;button<N_BUTTONS;button++){
                    if(elevator->order_matrix->matrix[floor][button]) return GOING_UP;
                }
            }
            for(int floor = elevator->current_floor;floor >= 0; floor--){
                for(int button=0;button<N_BUTTONS;button++){
                    if(elevator->order_matrix->matrix[floor][button]) return GOING_DOWN;
                }
            }
            break;
    }
    return DOORS_CLOSED;
}

Elevator *elevator_constructor(int floor) {
    Elevator *elevator = malloc(sizeof(Elevator));
    if (!elevator) {
        perror("malloc failed to make Elevator!\n");
        exit(1);
    }
    elevator->order_matrix = init_matrix();
    elevator->last_mving_state=DOORS_CLOSED;
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
