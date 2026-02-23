#include "orderMatrix.h"


void print_matrix(OrderMatrix* matrix){
    for(int floor=0;floor<N_FLOORS;floor++){
        for(int button=0;button<N_BUTTONS;button++){
            printf("%d ",matrix->matrix[floor][button]);
        }
        printf("\n");
    }
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