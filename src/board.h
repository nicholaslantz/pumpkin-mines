/* Board - Create and manage the game board
 *
 * Author: Nicholas Lantz
 * Date: <2016-0307>
 */

#pragma once

enum cell_type {CLEAR, MINE};
enum cell_status {HIDDEN, REVEALED, FLAGGED};
enum gamestate {NONE, UNDECIDED, DEFEAT, VICTORY};

struct cell {
    enum cell_type type;
    enum cell_status status;
    unsigned short num_mine_neighbors;

    short should_highlight;
};

struct row {
    struct cell *cells;
};

struct minesweeper_board {
    unsigned short num_rows, num_cols;
    struct row *rows;
    unsigned short num_mines;

    enum gamestate state;
};

struct minesweeper_board *generate_board(unsigned short rows,
        unsigned short cols, unsigned short num_mines);

enum gamestate reveal_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col);
void flag_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col);

void print_board_revealed(struct minesweeper_board *self);
void print_board_hidden(struct minesweeper_board *self);

char char_cell(struct cell *self, short should_hide);
char *str_cell_status(struct cell *self);
char *str_cell_type(struct cell *self);
char *str_gamestate(struct minesweeper_board *self);

char char_cell_type(enum cell_type self);
char char_cell_status(enum cell_status self);

void delboard(struct minesweeper_board *self);
