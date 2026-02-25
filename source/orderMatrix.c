#include "orderMatrix.h"


void print_matrix(OrderMatrix* matrix){
    for(int floor=0;floor<N_FLOORS;floor++){
        for(int button=0;button<N_BUTTONS;button++){
            printf("%d ",matrix->matrix[floor][button]);
        }
        printf("\n");
    }
}

void clear_matrix(OrderMatrix* matrix){
    for(int floor = 0; floor<N_FLOORS;floor++){
        for(int button=0; button<N_BUTTONS; button++){
            matrix->matrix[floor][button]=0;
            elevio_buttonLamp(floor, button, 0);
        }
    }
}

int check_if_order_below(OrderMatrix *matrix, int floorArgument){
    for(int floor = floorArgument;floor >= 0; floor--){
        for(int button=0;button<N_BUTTONS;button++){
            if(matrix->matrix[floor][button]) return 1;
        }
    }
    return 0;
}
int check_if_order_above(OrderMatrix *matrix, int floorArgument){
    for(int floor = floorArgument;floor < N_FLOORS; floor++){
        for(int button=0;button<N_BUTTONS;button++){
            if(matrix->matrix[floor][button]) return 1;
        }
    }
    return 0;
}
int check_if_order_at_floor(OrderMatrix *matrix, int floor, int button_to_be_ignored){
    for(int button=0;button<N_BUTTONS;button++){
        if (!(button==button_to_be_ignored) && matrix->matrix[floor][button]){
            return 1;
        }
    }
    return 0;
}

OrderMatrix *init_matrix(){
    OrderMatrix *order_matrix = malloc(sizeof(OrderMatrix));

    if (!order_matrix)
    {
        perror("malloc failed");
        exit(1);
    }

    for(int row =0;row<N_FLOORS;row++){
        for(int button=0;button<N_BUTTONS;button++){
            order_matrix->matrix[row][button]=0;
        }
    }

    return order_matrix;
}