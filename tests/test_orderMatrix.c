#include "../source/orderMatrix.h"
#include "../source/driver/elevio.h"
#include <stdlib.h>
#include <assert.h>


int main(){
    OrderMatrix *orderMatrix = init_matrix();

    for (int currentFloor=0;currentFloor<N_FLOORS;currentFloor++){
        for (int floor = 0; floor < N_FLOORS; floor++){
            for(int button = 0; button < N_BUTTONS; button++){
                orderMatrix->matrix[floor][button]=1;
                assert(check_if_order_above(orderMatrix, currentFloor)==(floor>=currentFloor));
                assert(check_if_order_below(orderMatrix, currentFloor)==(floor<=currentFloor));
                orderMatrix->matrix[floor][button]=0;
            }
        }
        assert(!check_if_order_above(orderMatrix, currentFloor));
        assert(!check_if_order_below(orderMatrix, currentFloor));
    }
    for (int floor = 0; floor < N_FLOORS; floor++){
        for(int button = 0; button < N_BUTTONS; button++){
            for(int ignoreButton = 0; ignoreButton < N_BUTTONS; ignoreButton++){
                orderMatrix->matrix[floor][button]=1;
                assert(check_if_order_at_floor(orderMatrix, floor, ignoreButton)==(button!=ignoreButton));
                orderMatrix->matrix[floor][button]=0;
            }
        }
    }
    for (int floor = 0; floor < N_FLOORS; floor++){
        for(int button = 0; button < N_BUTTONS; button++){
            orderMatrix->matrix[floor][button]=1;
        }
    }
    clear_matrix(orderMatrix);
    for (int floor = 0; floor < N_FLOORS; floor++){
        for(int button = 0; button < N_BUTTONS; button++){
            assert(orderMatrix->matrix[floor][button]==0);
        }
    }
    return 0;
}