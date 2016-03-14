

#include "ncurses_interface.h"

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
}

void draw_info(struct info_window *self) {
    box(self->win, 0, 0);

    wrefresh(self->win);
}
