

#include "ncurses_interface.h"


void draw_mine_row(struct board_window *self, struct row *r);
void draw_between_row(struct board_window *self);
void draw_top_row(struct board_window *self);
void draw_bottom_row(struct board_window *self);

void draw_board(struct board_window *self) {
    wclear(self->win);

    self->board->rows[self->user_y].cells[self->user_x].should_highlight = 1;
    struct row *r;

    draw_top_row(self);

    for(r = self->board->rows;
            r < self->board->rows + (self->board->num_rows-1); r++) {
        draw_mine_row(self, r);
        draw_between_row(self);
    }
    draw_mine_row(self, self->board->rows + (self->board->num_rows-1));
    draw_bottom_row(self);

    wprintw(self->win, "(%d, %d)", self->user_x, self->user_y);

    wrefresh(self->win);
    refresh();

    self->board->rows[self->user_y].cells[self->user_x].should_highlight = 0;
}

void draw_mine_row(struct board_window *self, struct row *r) {
    
    struct cell *c;
    for (c = r->cells; c < r->cells + self->board->num_cols; c++) {
        if (c->should_highlight) {
            wprintw(self->win, "%s ", "│");
            wattron(self->win, A_STANDOUT);
            waddch(self->win, char_cell(c, 0));
            wattroff(self->win, A_STANDOUT);
            waddch(self->win, ' ');
        } else {
            wprintw(self->win, "%s %c ", "│", char_cell(c, 0));
        }
    }

    wprintw(self->win, "%s\n", "│");
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
    unsigned int num_chars_to_print = (self->board->num_cols * 4) - 1;
    waddstr(self->win, "┌");

    unsigned int i;
    for(i = 0; i < num_chars_to_print; i++) {
        waddstr(self->win, "─");
    }

    waddstr(self->win, "┐\n");
}
void draw_bottom_row(struct board_window *self) {
    unsigned int num_chars_to_print = (self->board->num_cols * 4) - 1;
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
