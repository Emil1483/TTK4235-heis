#include "timer.h"
#include <assert.h>

int global_variable = 0;

void timer_fire(void *arg) {
    global_variable += 1;
    printf("Timer fired!\n");
}

int main() {
    Timer *p_timer = init_timer(NULL, timer_fire);

    // check that we fire within 0.1 seconds after delay
    reset(p_timer, 0.2);
    nanosleep(&(struct timespec){0, 100 * 1000 * 1000}, NULL);
    assert(global_variable == 0);
    nanosleep(&(struct timespec){0, 200 * 1000 * 1000}, NULL);
    assert(global_variable == 1);

    // check that we can cancel
    reset(p_timer, 0.2);
    nanosleep(&(struct timespec){0, 100 * 1000 * 1000}, NULL);
    assert(global_variable == 1);
    cancel(p_timer);
    nanosleep(&(struct timespec){0, 200 * 1000 * 1000}, NULL);
    assert(global_variable == 1);

    // check that reset actually resets
    reset(p_timer, 0.2);
    nanosleep(&(struct timespec){0, 100 * 1000 * 1000}, NULL);
    assert(global_variable == 1);
    reset(p_timer, 0.2);
    nanosleep(&(struct timespec){0, 100 * 1000 * 1000}, NULL);
    assert(global_variable == 1);
    reset(p_timer, 0.2);
    nanosleep(&(struct timespec){0, 100 * 1000 * 1000}, NULL);
    assert(global_variable == 1);
    nanosleep(&(struct timespec){0, 200 * 1000 * 1000}, NULL);
    assert(global_variable == 2);

    free(p_timer);
    return 0;
}