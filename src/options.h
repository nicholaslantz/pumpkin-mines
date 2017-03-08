
struct options {
    int rows, cols, mines;
    int debug;
    int error;
    char *rc_filename;
};

struct options get_cmdline(int argc, char **argv);
