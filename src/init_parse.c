

/* For this functionality, we need to load a file and load appropriate
 * variables with chars that will be used as controls.
 */
#include "init_parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char *VALID_KEYS[] = {
    "up", "down", "left", "right",
    "far_up", "far_down", "far_left", "far_right"
    "reveal", "flag", "debug", "far_amount"
};

const unsigned NUM_VALID_KEYS = 12;

struct controls load_init_file(const char *filename) {
    struct controls ret;

    // format:
    // control=key
    // Only one of these "declarations" per line
    
    char **lineptr = NULL;
    size_t *n = NULL;

    FILE *in = fopen(filename, "r");

    int line_number = 0;
    while (getline(lineptr, n, in)) {
        char *key, *val;

        line_number++;
        // This is user entered, so it will probably contain errors.
        // Should be nice and output messages

        // Check that the line contains an '='
        char *contains_eq = strstr(*lineptr, "=");

        if (!contains_eq) {
            fprintf(stderr, "Invalid entry: No '=' in line %d", line_number);
            ret.error = 1;
        }

        key = strtok(*lineptr, "=");
        val = strtok(NULL, "=");

        int key_found = 0;
        for (int i = 0; i < NUM_VALID_KEYS; i++) {
            if (key == VALID_KEYS[i]) {
                // incredibly sketchy hack, doubt it will work
                // TODO: Handle function, enter keys
                ((char*)&ret)[i] = val[0];
                key_found = 1;
            }
        }

        if (!key_found) {
            fprintf(stderr,
                    "Invalid Entry: %s not in valid keys, ignoring entry",
                    key);
        }

        free(*lineptr);
    }

    return ret;
}
