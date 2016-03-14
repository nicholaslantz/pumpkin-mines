

#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void generate_mines(struct minesweeper_board *self);
short place_mine(struct minesweeper_board *self);
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

    generate_mines(board);
    populate_cell_data(board);

    board->state = UNDECIDED;

    return board;
}

enum gamestate reveal_cell(struct minesweeper_board *self, unsigned short row,
        unsigned short col) {

    struct cell *c = &(self->rows[row].cells[col]);

    enum gamestate ret = UNDECIDED

    if (c->status == REVEALED) {
        ret = self->state;
        // return self->state because no change in gamestate
    } else {
        c->status = REVEALED;
        if (c->type == MINE) {
            ret = DEFEAT;
        } else if (c->num_mine_neighbors == 0) {
            // reveal 8 surrounding cells;

            short i, j;
            for(i = -1; i <= 1; i++) {
                for(j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) {continue;}
                    if (!in_bounds(self, row+i, col+j)) {continue;}

                    reveal_cell(self, row+i, col+j);
                }
            }
            ret = UNDECIDED;
        }
    }

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
        c->status = HIDDEN;
    } else if (c->status == HIDDEN) {
        c->status = FLAGGED;
    }
}

/* This function will not generate mines if the number of mines to place is
 * greater than number of tiles on board, it'll print an error message if the
 * caller tries to do that cause it's nice */

void generate_mines(struct minesweeper_board *self) {
    // naive implementation for now.
    if (self->num_mines > self->num_rows * self->num_cols) {
        fprintf(stderr, "Error: number of mines to place exceeds "
                 "total number of tiles\n");
        return;
    }

    unsigned short mines_to_place = self->num_mines;

    while (mines_to_place > 0) {
        if (place_mine(self)) {
            // successfully place mine
            mines_to_place--;
        } else {
            continue;
        }
    }
}


/* Attempts to place a mine, returns 1 on success, 0 on failure
 */
short place_mine(struct minesweeper_board *self) {
    short row = rand() % self->num_rows;
    short col = rand() % self->num_cols;

    //if (self->rows[row]->cells[col].type == MINE) {
    if (self->rows[row].cells[col].type == MINE) return 0;
    else {
        self->rows[row].cells[col].type = MINE;
        return 1;
    }
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
    if (row < 0 || col < 0) {
        return 0;
    }
    if (row > board->num_rows-1 || col > board->num_cols-1) {
        return 0;
    }

    return 1;
}

void print_board_revealed(struct minesweeper_board *self) {
    struct row *r;
    unsigned int i;
    unsigned int num_chars_to_print = (self->num_cols * 4) + 1;

    for(i = 0; i < num_chars_to_print; i++) {
        printf("%s", "─");
    }
    printf("\n");

    for(r=self->rows; r < self->rows + self->num_rows; r++) {
        
        struct cell *c;
        for(c = r->cells; c < r->cells + self->num_cols; c++) {
            printf("%s %c ", "│", char_cell(c, 0));
        }

        printf("%s\n", "│");
        for(i = 0; i < num_chars_to_print; i++) {
            printf("%s", "─");
        }

        printf("\n");
    }
}

char char_cell(struct cell *self, short should_hide) {
    if (should_hide) {
        if (self->status == HIDDEN) return ' ';
    }

    if (self->status == FLAGGED) return 'F';
    if (self->type == MINE) return 'M';
    if (self->num_mine_neighbors == 0) return '.';
    

    return (char) (self->num_mine_neighbors + ASCII_OFFSET);
}

char *str_cell_status(struct cell *self) {
    switch(self->status) {
        case HIDDEN:
            return strdup("Hidden");
        case REVEALED:
            return strdup("REVEALED");
        case FLAGGED:
            return strdup("FLAGGED");
        default:
            return strdup("ERROR");
    }
}

char *str_cell_type(struct cell *self) {
    switch (self->type) {
        case CLEAR:
            return strdup("CLEAR");
        case MINE:
            return strdup("MINE");
        default:
            return strdup("ERROR");
    }
}

char *str_gamestate(struct minesweeper_board *self) {
    switch (self->state) {
        case NONE:
            return strdup("NONE");
        case UNDECIDED:
            return strdup("UNDECIDED");
        case DEFEAT:
            return strdup("DEFEAT");
        case VICTORY:
            return strdup("VICTORY");
        default:
            return strdup("ERROR");
    }
}

char char_cell_type(enum cell_type self) {
    switch (self) {
        case CLEAR:
            return ' ';
        case MINE:
            return 'X';
        default:
            return '!';
    }
}

char char_cell_status(enum cell_status self) {
    switch (self) {
        case HIDDEN:
            return '.';
        case REVEALED:
            return ' ';
        case FLAGGED:
            return 'F';
        default:
            return '!';
    }
}

void delboard(struct minesweeper_board *self) {
    struct row *r;

    for(r = self->rows + (self->num_rows - 1); r >= self->rows; r--) {
        free(r->cells);
    }

    free (self->rows);
    free (self);
}
