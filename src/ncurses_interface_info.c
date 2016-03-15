
#include "ncurses_interface.h"
#include <stdio.h>
#include <stdlib.h>

void setup(struct board_window *boardwin, struct info_window *infowin,
           struct minesweeper_board *board) {
    initscr(); // start curses
    cbreak();  // all keyboard events sent to me
    raw();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    boardwin->startx = 0;
    boardwin->starty = 0;
    boardwin->width = COLS;
    boardwin->height = LINES - 3;
    curs_set(FALSE);

    boardwin->win = newwin(boardwin->height, boardwin->width,
                           boardwin->starty, boardwin->startx);
    boardwin->board = board;
    boardwin->user_x = 0;
    boardwin->user_y = 0;
    boardwin->infowin = infowin;

    infowin->startx = 0;
    infowin->starty = LINES - 3 - 1; // I assume 0-based indexing
    infowin->width = COLS;
    infowin->height = 3;

    infowin->win = newwin(infowin->height, infowin->width,
                           infowin->starty, infowin->startx);

    infowin->sw = (struct stopwatch *) malloc(sizeof(struct stopwatch));
    infowin->board = board;
    infowin->sw->elapsed_time.tv_nsec = -1; 

    if (has_colors() == 0) {
        // very elegant solution to the problem
        endwin();
        fprintf(stderr, "Mr. Werner, I demand color Mr. Werner!!!!\n");
        exit(EXIT_FAILURE);
    }

    start_color();

    assume_default_colors(COLOR_WHITE, COLOR_BLACK);
    init_color(COLOR_BLACK, 0, 0, 0);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_BLACK);
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(10, COLOR_BLACK, COLOR_WHITE);
}

void draw_info(struct info_window *self) {
    werase(self->win);
    if (self->sw->elapsed_time.tv_nsec != -1) {
        update_clock(self->sw);
    }

    wprintw(self->win, "%d", get_time_print(self->sw));

    wrefresh(self->win);
}
