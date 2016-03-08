/* Board - Create and manage the game board
 *
 * Author: Nicholas Lantz
 * Date: <2016-0307>
 */

enum cell_type {CLEAR, MINE};

struct cell {
    enum cell_type;
    unsigned short num_mine_neighbors;
    unsigned short row, col;
};

struct row {
    struct cell *thisrow;
}

struct minesweeper_board {
    size_t rows, cols;
    struct row *rows;
};
