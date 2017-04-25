

#pragma once

// Please note: the order of these variables actually matters for parsing the
// init file. Please don't change them. I should probably make it more robust,
// but I really don't care.
struct controls {
    int up, down, left, right;
    int far_up, far_down, far_left, far_right;
    int flag, reveal, newgame, debug;

    int error;
};

struct controls load_init_file(const char *filename);
