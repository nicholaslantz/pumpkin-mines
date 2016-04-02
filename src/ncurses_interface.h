

#pragma once
#include <ncurses.h>
#include "board.h"
#include "stopwatch.h"

typedef enum {UP, DOWN, LEFT, RIGHT} direction;
typedef enum {NORMAL, RED, GRAY, WHITE} draw_status;

struct info_window;

struct board_window {
    WINDOW *win;
    int startx, starty, width, height;

    struct minesweeper_board *board;
    struct info_window *infowin;

    int user_x, user_y;
    short debug;

    draw_status draw_stat;

};

struct info_window {
    WINDOW *win;
    int startx, starty, width, height;

    struct stopwatch *sw;
    struct minesweeper_board *board;
};

void setup(struct board_window *boardwin, struct info_window *infowin,
           struct minesweeper_board *board, short restart);

void draw_board();
void draw_info();

void shift_cursor(struct board_window *self, direction d);

/* Will reveal the location that the player's cursor is under
 */
void reveal(struct board_window *self);
void flag(struct board_window *self);
