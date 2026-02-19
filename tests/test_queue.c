#include "queue.h"
#include "elevio.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    Queue *p_queue = init_queue();

    printf("p_queue address: %p\n", p_queue);

    print_queue(p_queue);

    Order order;
    order.button_type = 0;
    order.floor_number = 0;

    place_order(p_queue, 0, order);

    print_queue(p_queue);
    assert(p_queue->length == 1);

    order.floor_number = 1;
    place_order(p_queue, 1, order);
    print_queue(p_queue);
    assert(p_queue->length == 2);

    order.floor_number = 2;
    place_order(p_queue, 1, order);
    print_queue(p_queue);
    assert(p_queue->length == 3);

    order.floor_number = 3;
    place_order(p_queue, 0, order);
    print_queue(p_queue);
    assert(p_queue->length == 4);

    order.floor_number = 3;
    order.button_type = 1;
    place_order(p_queue, 3, order);
    print_queue(p_queue);
    assert(p_queue->length == 5);

    remove_orders(p_queue, 3);
    print_queue(p_queue);
    assert(p_queue->length == 3);

    remove_orders(p_queue, 0);
    print_queue(p_queue);
    assert(p_queue->length == 2);

    free(p_queue->p_orders);
    free(p_queue);

    return 0;
}
