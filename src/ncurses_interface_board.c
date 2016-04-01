

#include "ncurses_interface.h"

void draw_mine_row(struct board_window *self, struct row *r);
void draw_between_row(struct board_window *self);
void draw_top_row(struct board_window *self);
void draw_bottom_row(struct board_window *self);

void draw_board(struct board_window *self) {
    werase(self->win);

    struct cell *cursor = &(self->board->rows[self->user_y].cells[self->user_x]);
    struct row *r;

    cursor->should_highlight = 1;

    draw_top_row(self);

    for(r = self->board->rows;
            r < self->board->rows + (self->board->num_rows-1); r++) {
        draw_mine_row(self, r);
        //draw_between_row(self);
    }
    draw_mine_row(self, self->board->rows + (self->board->num_rows-1));
    draw_bottom_row(self);

    if (self->debug) {
        // need to free memory over here
        wprintw(self->win, "(%d, %d)\n", self->user_x, self->user_y);
        wprintw(self->win, "Cell Type: %s\n", str_cell_type(cursor));
        wprintw(self->win, "Cell Status: %s\n", str_cell_status(cursor));
        wprintw(self->win, "Game State: %s\n", str_gamestate(self->board));
        wprintw(self->win, "Mine Neighbors: %d", cursor->num_mine_neighbors);
    }

    wrefresh(self->win);

    cursor->should_highlight = 0;
}

void print_cell(struct board_window *self, struct cell *c);
void print_revealed_cell(struct board_window *self, struct cell *c);

void draw_mine_row(struct board_window *self, struct row *r) {
    
    struct cell *c;
    wprintw(self->win, "%s", "│");
    for (c = r->cells; c < r->cells + self->board->num_cols; c++) {
        if (c->should_highlight) {
            wattron(self->win, A_BLINK);
            waddch(self->win, char_cell(c, 1));
            wattroff(self->win, A_BLINK);
        } else {
            print_cell(self, c);
        }
    }

    wprintw(self->win, "%s\n", "│");
}

void print_cell(struct board_window *self, struct cell *c) {
    switch (c->status) {
        case HIDDEN:
            wattron(self->win, COLOR_PAIR(10));
            wprintw(self->win, " ");
            wattroff(self->win, COLOR_PAIR(10));
            break;
        case REVEALED:
            print_revealed_cell(self, c);
            break;
        case FLAGGED:
            wattron(self->win, A_STANDOUT);
            wattron(self->win, COLOR_PAIR(3));
            wprintw(self->win, "F");
            wattroff(self->win, COLOR_PAIR(3));
            wattroff(self->win, A_STANDOUT);
            break;
    }
}

void print_revealed_cell(struct board_window *self, struct cell *c) {

    char ch = c->num_mine_neighbors + ASCII_OFFSET;
    switch (c->num_mine_neighbors) {
        case 0:
            //wattron(self->win, A_STANDOUT);
            waddch(self->win, ' ');
            //wattroff(self->win, A_STANDOUT);
            break;
        default:
            wattron(self->win, COLOR_PAIR(c->num_mine_neighbors));
            waddch(self->win, ch);
            wattroff(self->win, COLOR_PAIR(c->num_mine_neighbors));
            break;
    }

}

void draw_between_row(struct board_window *self) {
    waddstr(self->win, "├");

    unsigned int i;
    for (i = 0; i < self->board->num_cols-1; i++) {
        waddstr(self->win, "───┼");
    }

    waddstr(self->win, "───┤\n");
}
void draw_top_row(struct board_window *self) {
    unsigned int num_chars_to_print = (self->board->num_cols);
    waddstr(self->win, "┌");

    unsigned int i;
    for(i = 0; i < num_chars_to_print; i++) {
        waddstr(self->win, "─");
    }

    waddstr(self->win, "┐\n");
}
void draw_bottom_row(struct board_window *self) {
    unsigned int num_chars_to_print = (self->board->num_cols);
    waddstr(self->win, "└");

    unsigned int i;
    for(i = 0; i < num_chars_to_print; i++) {
        waddstr(self->win, "─");
    }

    waddstr(self->win, "┘\n");
}

void shift_cursor(struct board_window *self, direction d) {
    switch (d) {
        case UP:
            if (self->user_y > 0) self->user_y--;
            break;
        case DOWN:
            if (self->user_y < (self->board->num_rows - 1)) self->user_y++;
            break;
        case RIGHT:
            if (self->user_x < (self->board->num_cols - 1)) self->user_x++;
            break;
        case LEFT:
            if (self->user_x > 0) self->user_x--;
            break;
    }
}

void reveal(struct board_window *self) {
    enum gamestate result =
        reveal_cell(self->board, self->user_y, self->user_x);\

    if (self->infowin->sw->elapsed_time.tv_nsec == -1) {
        start_clock(self->infowin->sw);
        update_clock(self->infowin->sw);
    }

    switch (result) {
    case UNDECIDED:
        break;
    case DEFEAT:
        break;
    case VICTORY:
        break;
    default:
        break;
    }
}

void flag(struct board_window *self) {
    flag_cell(self->board, self->user_y, self->user_x);
}
