/* Board - Create and manage the game board
 *
 * Author: Nicholas Lantz
 * Date: <2016-0307>
 */

#pragma once

enum cell_type {CLEAR, MINE};
enum cell_status {hidden, revealed, flagged};

struct cell {
    enum cell_type type;
    enum cell_status status;
    unsigned short num_mine_neighbors;
    unsigned short row, col;
};

struct row {
    struct cell *thisrow;
}

struct minesweeper_board {
    unsigned short rows, cols;
    struct row *rows;
    unsigned short num_mines;
};

struct minesweeper_board generate_board(unsigned short rows,
        unsigned short cols, unsigned short num_mines);

void reveal_cell(minesweeper_board *self, unsigned short row,
        unsigned short col);
void flag_cell(minesweeper_board *self, unsigned short row,
        unsigned short col);
