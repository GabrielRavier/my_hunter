/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_end_round
*/

#include "../end_round.h"
#include "internal.h"
#include "my/macros.h"

void game_update_end_round(struct game *self)
{
    bool is_finished_sorting = true;

    for (size_t i = 0; i < (MY_ARRAY_SIZE(self->state.round.ducks) - 1); ++i)
        if (self->state.round.ducks[i].state == ROUND_DUCK_STATE_LIVES &&
            self->state.round.ducks[i + 1].state == ROUND_DUCK_STATE_DEAD) {
            is_finished_sorting = false;
            break;
        }
    if (!is_finished_sorting)
        game_update_end_round_before_sorting(self);
    else
        game_update_end_round_after_sorting(self);
}
