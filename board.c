

#include "board.h"

struct minesweeper_board generate_board(unsigned short rows,
       unsigned short cols, unsigned short num_mines) {

    struct minesweeper_board *board = (minesweeper_board *)
                                malloc(sizeof(minesweeper_board));
    board->rows = rows;
    board->cols = cols;
    board->num_mines = num_mines;
    board->rows = (row *) malloc(sizeof(row) * num_rows);

    row *r;
    for (r=board->rows; r < board->rows + rows; r++) {
        cell *c;

        for (c = r; c < r + cols; c++) {

        }
    }
}

void reveal_cell(minesweeper_board *self, unsigned short row,
        unsigned short col) {

}

void flag_cell(minesweeper_board *self, unsigned short row,
        unsigned short col) {

}
