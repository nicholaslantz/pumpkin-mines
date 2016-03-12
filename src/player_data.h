/* player_data - Maintain data on the player
 *
 * Author: Nicholas Lantz
 * Date: <2016-03-2017>
 */
#define TOTAL_TIMES_KEPT 10

#pragma once

struct player_data {
    unsigned int num_games;
    unsigned int num_wins;
    double best_times[TOTAL_TIMES_KEPT]

    char *filename;
}

void load_data(player_data *self);
void save_data(player_data *self);
void print_data(player_data *self);
