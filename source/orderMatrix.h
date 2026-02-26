#pragma once
#include "driver/elevio.h"
#include "stdlib.h"
#include "stdio.h"
#include <stdbool.h>

typedef struct
{
    int matrix[N_FLOORS][N_BUTTONS];
} OrderMatrix;
void print_matrix(OrderMatrix *matrix);
int check_if_order_below(OrderMatrix *matrix, int floorArgument);
int check_if_order_at_floor(OrderMatrix *matrix, int floor, int button_to_be_ignored);
int check_if_order_above(OrderMatrix *matrix, int floorArgument);
void clear_matrix(OrderMatrix *matrix);
bool matrix_is_empty(OrderMatrix *matrix);

OrderMatrix *init_matrix();