

// Please note: the order of these variables actually matters for parsing the
// init file. Please don't change them. I should probably make it more robust,
// but I really don't care.

// TODO: Support variables other than chars
struct controls {
    char up, down, left, right;
    char far_up, far_down, far_left, far_right;
    char flag, reveal, debug;

    int error;
};

struct controls load_init_file(const char *filename);
