#pragma once
#include "driver/elevio.h"
//#include "elevator.h"

typedef struct {
    int matrix[N_FLOORS][N_BUTTONS];
} OrderMatrix;
void print_matrix(OrderMatrix* matrix);
void place_matrix_order(OrderMatrix* matrix);

OrderMatrix *init_matrix();