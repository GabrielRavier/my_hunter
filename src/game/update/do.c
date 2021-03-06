/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implement game_update_do methods
*/

#include "do.h"
#include "../set/mode.h"
#include "my/macros.h"
#include <SFML/Audio/Sound.h>

void game_update_do_sounds_stop_if_no_ducks_flying(struct game *self)
{
    bool should_stop = true;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        if (self->state.session.ducks[i].state == DUCK_STATE_FLYING) {
            should_stop = false;
            break;
        }
    if (should_stop) {
        sfSound_stop(self->resources.sounds.flying.sf_sound);
        sfSound_stop(self->resources.sounds.duck.sf_sound);
    }
}

void game_update_do_change_to_end_session_if_all_ducks_dead(struct game *self)
{
    bool should_change = true;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        if (self->state.session.ducks[i].state != DUCK_STATE_DEAD &&
            self->state.session.ducks[i].state != DUCK_STATE_INACTIVE) {
            should_change = false;
            break;
        }
    if (should_change)
        game_set_mode(self, GAME_MODE_END_SESSION);
}
