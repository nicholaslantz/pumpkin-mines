

#pragma once
#include <ncurses.h>
#include "board.h"

typedef enum {UP, DOWN, LEFT, RIGHT} direction;

struct board_window {
    WINDOW *win;
    int startx, starty, width, height;

    struct minesweeper_board *board;

    int user_x, user_y;
};

struct info_window {
    WINDOW *win;
    int startx, starty, width, height;

    //information *info; eventually
};

void setup();

void draw_board();
void draw_info();

void shift_cursor(struct board_window *self, direction d);

/* Will reveal the location that the player's cursor is under
 */
void reveal(struct board_window *self);
void flag(struct board_window *self);
