

#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ncurses.h> // for endwin()

const char *str_cell_status[] = {"Hidden", "Revealed", "Flagged", "ERROR"};
const char *str_cell_type[] = {"Clear", "Mine", "Error"};
const char *str_gamestate[] = {"None", "Undecided", "Defeat", "Victory", "Error"};

void generate_mines(struct minesweeper_board *self, unsigned short row,
        unsigned short col);
short place_mine(struct minesweeper_board *self, unsigned short row,
        unsigned short col);
void get_num_mine_neighbors(struct cell *self, struct minesweeper_board *board,
        short row, short col);
short in_bounds(struct minesweeper_board *board, short row, short col);
void populate_cell_data(struct minesweeper_board *self);


struct minesweeper_board *generate_board(unsigned short rows,
       unsigned short cols, unsigned short num_mines) {

    struct minesweeper_board *board = (struct minesweeper_board *)
            malloc(sizeof(struct minesweeper_board));
    board->num_rows = rows;
    board->num_cols = cols;
    board->num_mines = num_mines;
    board->rows = (struct row *) malloc(sizeof(struct row) * board->num_rows);

    struct row *r;
    for (r=board->rows; r < board->rows + board->num_rows; r++) {
        r->cells = (struct cell *) malloc(sizeof(struct cell) * board->num_cols);
    }

    board->state = UNDECIDED;
    board->num_flagged = 0;
    board->num_tiles = board->num_cols * board->num_rows;
    board->num_revealed = 0;

    return board;
}

enum gamestate reveal_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col) {

    if (self->num_revealed == 0) {
        // first reveal implies generate mines
        generate_mines(self, row, col);
        populate_cell_data(self);
    }

    struct cell *c = &(self->rows[row].cells[col]);

    enum gamestate ret = UNDECIDED;

    switch (c->status) {
    case FLAGGED:
        // do nothing
        ret = self->state;
        break;
    case REVEALED:
        // potential chord
        ;
        if (c->num_mine_neighbors == 0) {
            ret = self->state;
            break;
        }

        short i, j;
        short num_flagged = 0;
        for(i = -1; i <= 1; i++) {
            for(j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;
                if (!in_bounds(self, row+i, col+j)) continue;

                if (self->rows[row+i].cells[col+j].status == FLAGGED)
                    num_flagged++;
            }
        }

        if (num_flagged == c->num_mine_neighbors) {
            // valid chord
            for(i = -1; i <= 1; i++) {
                for(j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    if (!in_bounds(self, row+i, col+j)) continue;
                    
                    if (self->rows[row+i].cells[col+j].status == HIDDEN) {
                        if (reveal_cell(self, row+i, col+j) == DEFEAT)
                            return DEFEAT;
                    }
                }
            }
        }
        ret = self->state;
        break;
    case HIDDEN:
        c->status = REVEALED;
        self->num_revealed++;
        if (c->type == MINE) ret = DEFEAT;
        else if (c->num_mine_neighbors == 0) {
            // reveal 8 surrounding cells;
            short i, j;
            for(i = -1; i <= 1; i++) {
                for(j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    if (!in_bounds(self, row+i, col+j)) continue;

                    reveal_cell(self, row+i, col+j);
                }
            }
            ret = UNDECIDED;
        }
    }

    if (self->num_tiles - self->num_revealed - self->num_mines == 0
        && ret != DEFEAT) ret = VICTORY;

    self->state = ret;
    return ret;
}

void flag_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col) {
    struct cell *c = &(self->rows[row].cells[col]);

    if (c->status == REVEALED) {
        // do nothing
        return;
    } else if (c->status == FLAGGED) {
        self->num_flagged--;
        c->status = HIDDEN;
    } else if (c->status == HIDDEN) {
        self->num_flagged++;
        c->status = FLAGGED;
    }
}

/* This function will not generate mines if the number of mines to place is
 * greater than number of tiles on board, it'll print an error message if the
 * caller tries to do that cause it's nice
 */

void generate_mines(struct minesweeper_board *self, unsigned short row,
        unsigned short col) {
    if (self->num_mines > self->num_tiles) {
        endwin();
        fprintf(stderr, "Error: number of mines to place exceeds "
                 "total number of tiles\n");
        return;
    }

    unsigned short mines_to_place = self->num_mines;

    while (mines_to_place > 0) {
        if (place_mine(self, row, col)) mines_to_place--;
         else continue;
        
    }
}

short is_close(struct minesweeper_board *self, unsigned short row,
        unsigned short col, unsigned short newrow, unsigned short newcol);

/* Attempts to place a mine, returns 1 on success, 0 on failure
 */
short place_mine(struct minesweeper_board *self, unsigned short row,
        unsigned short col) {
    unsigned short newrow = rand() % self->num_rows;
    unsigned short newcol = rand() % self->num_cols;

    if (self->rows[newrow].cells[newcol].type == MINE) return 0;
    else if (is_close(self, row, col, newrow, newcol)) return 0;
    else {
        self->rows[newrow].cells[newcol].type = MINE;
        return 1;
    }
}

short is_close(struct minesweeper_board *self, unsigned short row,
        unsigned short col, unsigned short newrow, unsigned short newcol) {

    signed int rowint = row; signed int colint = col;
    signed int newrowint = newrow; signed int newcolint = newcol;

    return (abs(rowint - newrowint) < 3 && abs(colint - newcolint) < 3);
}

void populate_cell_data(struct minesweeper_board *self) {

    unsigned short i, j;
    for(i = 0; i < self->num_rows; i++) {
        for(j = 0; j < self->num_cols; j++) {
            struct cell *c = ((self->rows + i)->cells + j);
            c->status = HIDDEN;
            c->should_highlight = 0;
            get_num_mine_neighbors(c, self, i, j);
        }
    }
}

void get_num_mine_neighbors(struct cell *self, struct minesweeper_board *board,
        short row, short col) {

    self->num_mine_neighbors = 0;

    static short i, j;
    for(i = -1; i <= 1; i++) {
        for(j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            if (!in_bounds(board, row+i, col+j)) continue;

            if (board->rows[row+i].cells[col+j].type == MINE) {
                self->num_mine_neighbors++;
            }
        }
    }
}

short in_bounds(struct minesweeper_board *board, short row, short col) {
    if (row < 0 || row >= board->num_rows) return 0;
    if (col < 0 || col >= board->num_cols) return 0;
    return 1;
}



void delboard(struct minesweeper_board *self) {
    struct row *r;

    for(r = self->rows + (self->num_rows - 1); r >= self->rows; r--) {
        free (r->cells);
    }

    free (self->rows);
    free (self);
}
