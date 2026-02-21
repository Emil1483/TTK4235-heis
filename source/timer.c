#include "timer.h"
#include <math.h>

Timer *init_timer() {
    Timer *p_timer = malloc(sizeof(Timer));
    p_timer->has_started = false;
    p_timer->is_done = false;
    return p_timer;
}

void *timer_function(void *arg) {
    Timer *p_timer = (Timer *)arg;
    int seconds = (int)floor(p_timer->delay);
    float fractional_part = p_timer->delay - seconds;
    int nanoseconds = (int)floor(fractional_part * 1000 * 1000 * 1000);
    printf("sleeping for %i seconds and %i nanoseconds\n", seconds,
           nanoseconds);
    nanosleep(&(struct timespec){seconds, nanoseconds}, NULL);
    p_timer->on_fire();
    p_timer->is_done = true;
    return NULL;
}

void reset(Timer *timer, float delay) {

    if (timer->has_started && !timer->is_done) {
        cancel(timer);
    }

    timer->delay = delay;
    timer->has_started = true;
    timer->is_done = false;

    if (pthread_create(&timer->thread_id, NULL, timer_function, timer) != 0) {
        perror("Failed to create thread");
        exit(1);
    }
}

void cancel(Timer *timer) {
    printf("cancelling timer\n");
    pthread_cancel(timer->thread_id);
    pthread_join(timer->thread_id, NULL);
}