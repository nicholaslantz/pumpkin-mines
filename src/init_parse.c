

/* For this functionality, we need to load a file and load appropriate
 * variables with chars that will be used as controls.
 */
#include "init_parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

const char *VALID_KEYS[] = {
    "up", "down", "left", "right",
    "far_up", "far_down", "far_left", "far_right",
    "flag", "reveal", "newgame", "debug"
};

const unsigned NUM_VALID_KEYS = 12;

struct controls load_init_file(const char *filename) {
    struct controls ret = (struct controls) {
        'h', 'j', 'k', 'l', 'H', 'J', 'K', 'L', 'f', 'e', 'm', 'n', 0
    };

    if (! filename) {
        // If user did not supply a filename, set default values
        return ret;
    }

    // format:
    // control=key
    // Only one of these "declarations" per line

    FILE *in = fopen(filename, "r");

    if (! in) {
        fprintf(stderr, "Error loading %s, does this file exist?\n", filename);
        ret.error = 1;
        return ret;
    }

    // I have C programmer's disease. 1024 bytes should be sufficient here
    int line_size = 1024;
    char line[1024];

    int line_number = 0;
    while (fgets(line, line_size, in)) {
        char *key, *val;

        line_number++;
        // This is user entered, so it will probably contain errors.
        // Should be nice and output messages

        // Check that the line contains an '='
        char *contains_eq = strstr(line, "=");

        if (!contains_eq) {
            fprintf(stderr, "Invalid entry: No '=' in line %d\n", line_number);
            ret.error = 1;
        }

        key = strtok(line, "=");
        val = strtok(NULL, "=");

        int key_found = 0;
        for (int i = 0; i < NUM_VALID_KEYS; i++) {
            if (strcmp(key, VALID_KEYS[i]) == 0) {
                //TODO: Handle function, enter keys

                // incredibly sketchy hack, doubt it will work
                // it works
                ((int*)&ret)[i] = val[0];
                key_found = 1;
                break;
            }
        }

        if (!key_found) {
            fprintf(stderr,
                    "Invalid Entry on line %d: %s not in valid keys\n",
                    line_number, key);
            ret.error = 1;
        }

    }

    fclose(in);
    return ret;
}
