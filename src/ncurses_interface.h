

#pragma once
#include <ncurses.h>
#include "board.h"

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
