
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

    boardwin->startx = 0;
    boardwin->starty = 0;
    boardwin->width = COLS;
    boardwin->height = LINES - 3;

    boardwin->win = newwin(boardwin->height, boardwin->width,
                           boardwin->starty, boardwin->startx);
    boardwin->board = board;
    boardwin->user_x = 0;
    boardwin->user_y = 0;

    infowin->startx = 0;
    infowin->starty = LINES - 3 - 1; // I assume 0-based indexing
    infowin->width = COLS;
    infowin->height = 3;

    infowin->win = newwin(infowin->height, infowin->width,
                           infowin->starty, infowin->startx);

    if (has_colors() == 0) {
        // very elegant solution to the problem
        endwin();
        fprintf(stderr, "Mr. Werner, I demand color Mr. Werner!!!!\n");
        exit(EXIT_FAILURE);
    }

    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_BLACK);
    init_pair(8, COLOR_MAGENTA, COLOR_BLACK);
}

void draw_info(struct info_window *self) {
    box(self->win, 0, 0);

    wrefresh(self->win);
}
