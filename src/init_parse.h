

struct controls {
    char up, down, left, right;
    char far_up, far_down, far_left, far_right;
    char flag, reveal, debug;
    int far_amount;
    int error;
};

struct controls load_init_file(const char *filename);
