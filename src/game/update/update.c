/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update
*/

#include "../update.h"
#include "do.h"
#include "end_round.h"
#include "end_session.h"
#include "start_round.h"
#include "title.h"
#include "../change_music.h"
#include "../set/mode.h"
#include "../dog.h"
#include "../session_duck/update.h"
#include "../round_duck/update.h"
#include <SFML/Graphics/Sprite.h>
#include "my/macros.h"

static const uintmax_t FRAME_FLY_AWAY_START = 260;

static bool are_there_flying_ducks(const struct game_session_state *self)
{
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
        if (self->ducks[i].state == DUCK_STATE_FLYING)
            return (true);
    return (false);
}

static void do_session_or_fly_away(struct game *self)
{
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY) {
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
            session_duck_update(&self->state.session.ducks[i], self);
        game_update_do_sounds_stop_if_no_ducks_flying(self);
        game_update_do_change_to_end_session_if_all_ducks_dead(self);
    }
}

static void do_round_ducks_if_appropriate(struct game *self)
{
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_START_ROUND ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY ||
        self->state.mode == GAME_MODE_END_SESSION ||
        self->state.mode == GAME_MODE_END_ROUND)
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
            round_duck_update(&self->state.round.ducks[i], &self->state, i);
}

void game_update(struct game *self)
{
    if (self->state.mode == GAME_MODE_TITLE)
        game_update_title(self);
    if (self->state.mode == GAME_MODE_START_ROUND)
        game_update_start_round(self);
    if (self->state.mode == GAME_MODE_SESSION &&
        self->state.frames_since_mode_begin > FRAME_FLY_AWAY_START &&
        are_there_flying_ducks(&self->state.session))
        game_set_mode(self, GAME_MODE_SESSION_FLY_AWAY);
    do_session_or_fly_away(self);
    do_round_ducks_if_appropriate(self);
    if (self->state.mode == GAME_MODE_END_SESSION)
        game_update_end_session(self);
    if (self->state.mode == GAME_MODE_END_ROUND)
        game_update_end_round(self);
}
