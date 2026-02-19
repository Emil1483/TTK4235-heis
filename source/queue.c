#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue *init_queue()
{
    Queue *queue = malloc(sizeof(Queue));

    if (!queue)
    {
        perror("malloc failed");
        exit(1);
    }

    queue->length = 0;
    queue->p_orders = NULL;
    return queue;
}

void place_order(Queue *queue, int index, Order order)
{
    if (index < 0 || index > queue->length)
    {
        printf("index %i out of bounds [0, %i]\n",
               index, queue->length);
        exit(1);
    }

    queue->length++;

    if (queue->p_orders == NULL)
    {
        queue->p_orders = malloc(sizeof(Queue));
        queue->p_orders[0] = order;
    }
    else
    {
        queue->p_orders = realloc(queue->p_orders,
                                  queue->length * sizeof(Order));

        Order current = order;
        Order next = queue->p_orders[index];
        for (int i = index; i < queue->length; i++)
        {
            queue->p_orders[i] = current;
            if (i != queue->length - 1)
            {
                current = next;
                next = queue->p_orders[i + 1];
            }
        }
    }
}

Order get(Queue *queue, int index)
{
    if (queue->length == 0)
    {
        printf("the queue is empty. Cannot call get");
        exit(1);
    }

    if (index < 0 || index >= queue->length)
    {
        printf("index %i out of bounds [0, %i]\n",
               index, queue->length - 1);
        exit(1);
    }

    return queue->p_orders[index];
}

void print_queue(Queue *queue)
{
    printf("--- Queue (len: %i) ---\n", queue->length);

    if (queue->length == 0)
    {
        printf("(empty)\n");
    }

    for (int i = 0; i < queue->length; i++)
    {
        Order order = get(queue, i);
        printf("%i | button_type: %i, floor_number: %i\n",
               i, order.button_type, order.floor_number);
    }
    printf("---Queue---\n\n");
}

void remove_orders(Queue *queue, int floor_number)
{
    int new_size = 0;

    for (int i = 0; i < queue->length; i++)
    {
        if (queue->p_orders[i].floor_number != floor_number)
        {
            new_size++;
        }
    }

    Order *p_new_orders = malloc(new_size * sizeof(Queue));

    int j = 0;
    for (int i = 0; i < queue->length; i++)
    {
        if (queue->p_orders[i].floor_number != floor_number)
        {
            p_new_orders[j] = queue->p_orders[i];
            j++;
        }
    }

    free(queue->p_orders);
    queue->p_orders = p_new_orders;
    queue->length = new_size;
}