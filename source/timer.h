#pragma once

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    bool has_started;
    bool is_done;
    pthread_t thread_id;
    float delay;
    void (*on_fire)(void *on_fire_arg);
    void *on_fire_arg;
} Timer;

Timer *init_timer(void *on_fire_arg, void (*on_fire)(void *on_fire_arg));
void reset(Timer *timer, float delay);
void cancel(Timer *timer);