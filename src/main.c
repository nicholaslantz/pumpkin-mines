

#include "board.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));

    struct minesweeper_board *game_board = generate_board(16, 30, 99);

    print_board_revealed(game_board);

    exit(EXIT_SUCCESS);
}
