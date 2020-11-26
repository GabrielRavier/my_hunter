/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_end_round_after_sorting
*/

#include "internal.h"
#include "my/macros.h"

static const size_t ROUNDS_THRESHOLD_REQUIRED[19] = {6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 7, 7, 8, 8, 9, 9, 9, 9, 9};
static const size_t ROUNDS_THRESHOLD_REQUIRED_AFTER_20 = 10;

void game_update_end_round_after_sorting(struct game *self)
{
    size_t killed_ducks;

    if (self->state.round.frame_finished_sorting == -1)
        self->state.round.frame_finished_sorting =
            self->state.frames_since_mode_begin;
    for (killed_ducks = 0;
        killed_ducks < MY_ARRAY_SIZE(self->state.round.ducks); ++killed_ducks)
        if ((self->state.round.ducks[killed_ducks].state !=
                ROUND_DUCK_STATE_DEAD) &&
            (self->state.round.ducks[killed_ducks].state !=
                ROUND_DUCK_STATE_END_ROUND_FLASHING))
            break;
    if (killed_ducks < ((size_t)self->state.current_round.as_int <
        (MY_ARRAY_SIZE(ROUNDS_THRESHOLD_REQUIRED) - 1) ?
        ROUNDS_THRESHOLD_REQUIRED[self->state.current_round.as_int - 1] :
        ROUNDS_THRESHOLD_REQUIRED_AFTER_20))
        game_update_end_round_game_over(self);
    else
        game_update_end_round_won(self, killed_ducks);
}
