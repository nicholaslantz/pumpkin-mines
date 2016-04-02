

#pragma once
#include <time.h>

struct stopwatch {
    struct timespec time_begin;
    struct timespec time_current;

    struct timespec elapsed_time; // seconds

    short stopped;
};

void start_clock(struct stopwatch *self);
void update_clock(struct stopwatch *self);
void stop_clock(struct stopwatch *self);
int get_time_print(struct stopwatch *self);
