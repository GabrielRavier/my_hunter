/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_end_round_before_sorting
*/

#include "internal.h"
#include "my/macros.h"
#include <SFML/Audio/Sound.h>

static const int FRAMES_PER_SORT_STEP = 17;

void game_update_end_round_before_sorting(struct game *self)
{
    if (((self->state.frames_since_mode_begin % FRAMES_PER_SORT_STEP) == 0)) {
        sfSound_play(self->resources.sounds.round_ducks_move.sf_sound);
        for (size_t i = 0; i < (MY_ARRAY_SIZE(self->state.round.ducks) - 1);
            ++i)
            if (self->state.round.ducks[i].state == ROUND_DUCK_STATE_LIVES &&
                self->state.round.ducks[i + 1].state == ROUND_DUCK_STATE_DEAD) {
                self->state.round.ducks[i].state = ROUND_DUCK_STATE_DEAD;
                self->state.round.ducks[i + 1].state = ROUND_DUCK_STATE_LIVES;
            }
    }
}
