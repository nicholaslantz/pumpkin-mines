
#include "ncurses_interface.h"
#include <stdio.h>
#include <stdlib.h>

void setup(struct board_window *boardwin, struct info_window *infowin,
           struct minesweeper_board *board, short restart) {

    // Must call this function to begin using ncurses. Sets up the initial
    // ncurses environment
    initscr();

    if (!has_colors()) {
        // very elegant solution to the problem
        endwin();
        fprintf(stderr, "Mr. Werner, I demand color Mr. Werner!!!!\n");
        exit(EXIT_FAILURE);
    }

    start_color();

    // Disables line buffering and erase/kill character processing for the
    // terminal. Should make it run faster
    raw();

    // Disables the terminal's native line feed capabilities. This will =
    // make ncurses run a little faster
    nonl();

    // This will cause the user's input to not appear on the screen. This
    // is what we want. Don't want the user's keys to overwrite the game
    // board or anything.
    noecho();

    // By default, input will be flushed when an interrupt is fired. This
    // function with FALSE disables this functionality
    //intrflush(stdscr, FALSE);
    timeout(0);

    // This causes getch() to not block until input is received. If we did not
    // use this, the timer would not update until the user enters a key.
    nodelay(stdscr, TRUE);

    // Causes the terminal to hide the cursor. pumpkin-mines uses its own
    // cursor "system", so the terminal's is not necessary
    curs_set(FALSE);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(10, COLOR_BLACK, COLOR_WHITE);
    init_pair(11, COLOR_BLACK, COLOR_CYAN);
    boardwin->startx = 0;
    boardwin->starty = 0;
    boardwin->width = COLS;
    boardwin->height = LINES - 1;

    boardwin->win = newwin(boardwin->height, boardwin->width,
                           boardwin->starty, boardwin->startx);
    boardwin->board = board;
    boardwin->user_x = (board->num_cols / 2) - 1;
    boardwin->user_y = (board->num_rows / 2) - 1;
    boardwin->infowin = infowin;

    infowin->startx = 0;
    infowin->starty = LINES - 1;
    infowin->width = COLS;
    infowin->height = 1;

    infowin->win = newwin(infowin->height, infowin->width,
                          infowin->starty, infowin->startx);

    if (!restart)
        infowin->sw = (struct stopwatch *) malloc(sizeof(struct stopwatch));

    infowin->board = board;
    infowin->sw->elapsed_time.tv_nsec = -1; 
}

void draw_info(struct info_window *self) {
    werase(self->win);
    if (self->sw->elapsed_time.tv_nsec != -1) {
        update_clock(self->sw);
    }

    /* TODO: Make the program calculate the number of spaces for
     *       the marked section. Now, assuming that it needs 2 spaces
     *       because 99 is the hardcoded amount of mines
     */
    wprintw(self->win,
            "[%3d] Marked: %2d/%2d",
            get_time_print(self->sw),
            self->board->num_flagged,
            self->board->num_mines);
    if (self->board->state == DEFEAT)
        wprintw(self->win, " BOOM!");
    else if (self->board->state == VICTORY)
        wprintw(self->win, " Victory!");

    wrefresh(self->win);
}
