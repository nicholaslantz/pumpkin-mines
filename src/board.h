/* Board - Create and manage the game board
 *
 * Author: Nicholas Lantz
 * Date: <2016-03-07>
 */

#pragma once

static const unsigned short ASCII_OFFSET = 48;
extern const char *str_cell_status[];
extern const char *str_cell_type[];
extern const char *str_gamestate[];

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
    unsigned short is_close_radius;

    enum gamestate state;

    // shorts below make it easier to check if the player won each time
    // they reveal a tile
    unsigned short num_flagged;
    unsigned short num_tiles;
    unsigned short num_revealed;

};

/* generate_board will NOT place mines, call it to prepare the board for play.
 * Revealing a single mine will cause the rest of the mines to be generated, no
 * other action is required of the caller.
 */
struct minesweeper_board *generate_board(unsigned short rows,
        unsigned short cols, unsigned short num_mines,
        unsigned short is_close_radius);

void generate_mines(struct minesweeper_board *self, unsigned short row,
        unsigned short col);

enum gamestate reveal_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col);

void flag_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col);

void print_board_revealed(struct minesweeper_board *self);

void delboard(struct minesweeper_board *self);
