

#include "board.h"
#include "ncurses_interface.h"
#include "options.h"

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    setlocale(LC_ALL, "en_US.UTF-8");

    struct options opts = get_cmdline(argc, argv);

    if (opts.error) {
        fprintf(stderr, "Try pumpkin-mines --help for more information\n");
        return 1;
    }
    struct board_window boardwin;
    struct info_window infowin;

    struct minesweeper_board *game_board =
        generate_board(opts.rows,
                       opts.cols,
                       opts.mines);

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
            int shift_amount = 1;
            if (ch <= 122 && isupper(ch)) {
                ch = tolower(ch);
                shift_amount = 5;
            }

            switch (ch) {
            case KEY_UP:
            case 'k':
            case 'w':
                shift_cursor(&boardwin, UP, shift_amount);
                break;
            case KEY_DOWN:
            case 'j':
            case 's':
                shift_cursor(&boardwin, DOWN, shift_amount);
                break;
            case KEY_LEFT:
            case 'h':
            case 'a':
                shift_cursor(&boardwin, LEFT, shift_amount);
                break;
            case KEY_RIGHT:
            case 'l':
            case 'd':
                shift_cursor(&boardwin, RIGHT, shift_amount);
                break;
            case KEY_ENTER:
            case 'e':
            case ',':
                reveal(&boardwin);
                break;
            case ' ':
            case 'f':
            case '.':
                flag(&boardwin);
                break;
            case KEY_F(3):
                boardwin.debug = !boardwin.debug;
                break;
            case 'q':
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
