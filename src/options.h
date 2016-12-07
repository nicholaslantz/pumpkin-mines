
struct options {
    int rows, cols, mines;
    int debug;
    int error;
};

struct options get_cmdline(int argc, char **argv);
