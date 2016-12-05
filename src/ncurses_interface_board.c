

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
    }
    draw_mine_row(self, self->board->rows + (self->board->num_rows-1));
    draw_bottom_row(self);

    if (self->debug) {
        wprintw(self->win, "(%d, %d)\n", self->user_x, self->user_y);
        wprintw(self->win, "Cell Type: %s\n", str_cell_type[cursor->type]);
        wprintw(self->win, "Cell Status: %s\n", str_cell_status[cursor->status]);
        wprintw(self->win, "Game State: %s\n", str_gamestate[self->board->state]);
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
            wattron(self->win, A_STANDOUT);
            print_cell(self, c);
            wattroff(self->win, A_STANDOUT);
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
            if (self->board->state == DEFEAT && c->type != MINE) {
                wattron(self->win, A_STANDOUT);
                wattron(self->win, COLOR_PAIR(5));
                wprintw(self->win, "F");
                wattroff(self->win, COLOR_PAIR(5));
                wattroff(self->win, A_STANDOUT);
            } else {
                wattron(self->win, COLOR_PAIR(11));
                wprintw(self->win, "F");
                wattroff(self->win, COLOR_PAIR(11));
            }
            break;
    }
}

void print_revealed_cell(struct board_window *self, struct cell *c) {

    if (c->type == MINE) {
        waddch(self->win, 'M');
        return;
    }

    char ch = c->num_mine_neighbors + ASCII_OFFSET;
    switch (c->num_mine_neighbors) {
        case 0:
            //wattron(self->win, A_STANDOUT);
            waddch(self->win, ' ');
            //wattroff(self->win, A_STANDOUT);
            break;
        default:
            wattron(self->win, A_DIM);
            wattron(self->win, COLOR_PAIR(c->num_mine_neighbors));
            waddch(self->win, ch);
            wattroff(self->win, COLOR_PAIR(c->num_mine_neighbors));
            wattroff(self->win, A_DIM);
            break;
    }

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

void shift_cursor(struct board_window *self, direction d, int amount) {
    switch (d) {
        case UP:
            self->user_y -= amount;
            if (self->user_y < 0)
                self->user_y = 0;
            break;
        case DOWN:
            self->user_y += amount;
            if (self->user_y > (self->board->num_rows - 1))
                self->user_y = self->board->num_rows - 1;
            break;
        case RIGHT:
            self->user_x += amount;
            if (self->user_x > (self->board->num_cols - 1))
                self->user_x = self->board->num_cols - 1;
            break;
        case LEFT:
            self->user_x -= amount;
            if (self->user_x < 0)
                self->user_x = 0;
            break;
    }
}

void reveal(struct board_window *self) {
    if (self->board->state != UNDECIDED) return;

    enum gamestate result =
        reveal_cell(self->board, self->user_y, self->user_x);

    if (self->infowin->sw->elapsed_time.tv_nsec == -1) {
        start_clock(self->infowin->sw);
        update_clock(self->infowin->sw);
    }

    switch (result) {
    case UNDECIDED:
        break;
    case DEFEAT:
        stop_clock(self->infowin->sw);
        break;
    case VICTORY:
        stop_clock(self->infowin->sw);
        break;
    default:
        break;
    }
}

void flag(struct board_window *self) {
    if (self->board->state != UNDECIDED) return;
    flag_cell(self->board, self->user_y, self->user_x);
}
