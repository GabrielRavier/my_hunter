/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_round_state
*/

#pragma once

#include "round_duck.h"
#include <stdint.h>
#include <stddef.h>

// This is the state for a round
// current_ducks_index is the index of the ducks we're currently working with in
// terms of the bottom of the screen, e.g. if we've already done 3 sessions in 2
// ducks mode, this will be 5
// frame_finished_sorting is used by GAME_END_ROUND to determine on which frame
// it finished doing the sorting of all the dead ducks
// ducks is the ducks from the bottom of the screen
struct game_round_state {
    size_t current_ducks_index;
    intmax_t frame_finished_sorting;
    struct round_duck ducks[10];
};
