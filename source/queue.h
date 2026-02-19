#pragma once

#include "order.h"
#include <stdint.h>

typedef struct
{
    __uint8_t length;
    Order *p_orders;
} Queue;

Queue *init_queue();

void place_order(Queue *queue, int index, Order order);
void remove_orders(Queue *queue, int floor_number);
Order get(Queue *queue, int index);
void print_queue(Queue *queue);