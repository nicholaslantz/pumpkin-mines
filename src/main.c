

#include "board.h"
#include "ncurses_interface.h"
#include "options.h"
#include "init_parse.h"

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
    srand(time(NULL));

    setlocale(LC_ALL, "");

    struct options opts = get_cmdline(argc, argv);

    if (opts.error) {
        fprintf(stderr,
                "Try %s --help for more information\n",
                argv[0]);
        return 1;
    }

    struct controls ctrls = load_init_file(opts.rc_filename);

    if (ctrls.error) {
        fprintf(stderr, "Errors encountered while parsing config file\n");
        fprintf(stderr, "Please revise and try again\n");
        return 1;
    }

    struct board_window boardwin;
    struct info_window infowin;

    struct minesweeper_board *game_board =
        generate_board(opts.rows,
                       opts.cols,
                       opts.mines,
                       2);

    setup(&boardwin, &infowin, game_board, 0);
    boardwin.debug = opts.debug;

    // So, ncurses is not capable of detecting modifier keys by default. I
    // don't think it's standard that terminal applications detect them, so
    // I'm not going to do any sketchy hacks to get them to work. Instead,
    // to shift cursor by a larger amount you will need to input a capital
    // character. That is, you MUST use wasd or hjkl to go farther than a
    // single tile. Sorry arrow keys users.

    int ch;
    for (;;) {
        if ((ch = getch()) > 0) {
            // This used to be a switch, now that the controls are variables it
            // must be an if-else chain.
            if (ch == ctrls.up) {
                shift_cursor(&boardwin, UP, 1);
            } else if (ch == ctrls.down) {
                shift_cursor(&boardwin, DOWN, 1);
            } else if (ch == ctrls.left) {
                shift_cursor(&boardwin, LEFT, 1);
            } else if (ch == ctrls.right) {
                shift_cursor(&boardwin, RIGHT, 1);
            } else if (ch == ctrls.far_up) {
                shift_cursor(&boardwin, UP, 5);
            } else if (ch == ctrls.far_down) {
                shift_cursor(&boardwin, DOWN, 5);
            } else if (ch == ctrls.far_left) {
                shift_cursor(&boardwin, LEFT, 5);
            } else if (ch == ctrls.far_right) {
                shift_cursor(&boardwin, RIGHT, 5);
            } else if (ch == ctrls.reveal) {
                reveal(&boardwin);
            } else if (ch == ctrls.flag) {
                flag(&boardwin);
            } else if (ch == KEY_F(3)) {
                boardwin.debug = !boardwin.debug;
            } else if (ch == 'q') {
                goto end;
            }
        }
        draw_board(&boardwin);
        draw_info(&infowin);
        usleep(20000);
    }

    end:
    delboard(game_board);

    endwin();

    exit(EXIT_SUCCESS);
}
