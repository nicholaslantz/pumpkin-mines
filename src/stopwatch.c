

#include "stopwatch.h"
#include <sys/time.h>

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void start_clock(struct stopwatch *self) {
    self->stopped = 0;
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &(self->time_begin)) != 0) {
        endwin();
        perror("Error updating time");
        exit(EXIT_FAILURE);
    }
}

void update_clock(struct stopwatch *self) {
    if (!self->stopped) {
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &(self->time_current)) != 0) {
            endwin();
            perror("Error updating time");
            exit(EXIT_FAILURE);
        }

        self->elapsed_time.tv_nsec = self->time_current.tv_nsec
                                     - self->time_begin.tv_nsec;
        self->elapsed_time.tv_sec = self->time_current.tv_sec
                                     - self->time_begin.tv_sec;
    }
}

void stop_clock(struct stopwatch *self) {
    update_clock(self);
    self->stopped = 1;
}

int get_time_print(struct stopwatch *self) {
    return (int) self->elapsed_time.tv_sec;
}
