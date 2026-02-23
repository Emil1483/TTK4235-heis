#pragma once
#include "driver/elevio.h"

typedef struct {
    int matrix[N_FLOORS][N_BUTTONS];
} OrderMatrix;
void print_matrix(OrderMatrix* matrix);

OrderMatrix *init_matrix();