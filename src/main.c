

#include "board.h"
#include "ncurses_interface.h"

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    setlocale(LC_ALL, "en_US.UTF-8");

    struct board_window boardwin;
    struct info_window infowin;

    struct minesweeper_board *game_board = generate_board(16, 30, 99);

    setup(&boardwin, &infowin, game_board, 0);

    if (argc > 1 && (!strcmp(argv[1], "--debug") || !strcmp(argv[1], "-d")))
        boardwin.debug = 1;
    else boardwin.debug = 0;


    int ch;
    for (;;) {
        if ((ch = getch()) > 0) {
            switch (ch) {
            case KEY_UP:
            case 'k':
            case 'w':
                shift_cursor(&boardwin, UP);
                break;
            case KEY_DOWN:
            case 'j':
            case 's':
                shift_cursor(&boardwin, DOWN);
                break;
            case KEY_LEFT:
            case 'h':
            case 'a':
                shift_cursor(&boardwin, LEFT);
                break;
            case KEY_RIGHT:
            case 'l':
            case 'd':
                shift_cursor(&boardwin, RIGHT);
                break;
            case KEY_ENTER:
            case 'e':
                reveal(&boardwin);
                break;
            case ' ':
            case 'f':
                flag(&boardwin);
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
