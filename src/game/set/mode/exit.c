/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_exit_mode
*/

#include "exit.h"
#include "../current/round.h"
#include "../current/score.h"
#include "my/macros.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Audio/Sound.h>

static const sfIntRect GAMEPLAY_BACKGROUND_RECT = {0, 8, 255, 224};

// We start at round 0 : start_round will increase the round by 1 when
// initialized
static void game_exit_title(struct game *self)
{
    game_set_current_round(&self->state.current_round, 0);
    game_set_current_score(&self->state.scores, 0);
    sfSprite_setTextureRect(self->resources.sprites.gameplay_background,
        GAMEPLAY_BACKGROUND_RECT);
}

static void game_exit_session(struct game_sounds *sounds)
{
    sfSound_stop(sounds->duck.sf_sound);
    sfSound_stop(sounds->flying.sf_sound);
}

static void game_exit_end_session_or_revenge(struct game_state *state,
    enum game_mode *new_mode, struct game_sounds *sounds)
{
    sfSound_stop(sounds->dog_mocking.sf_sound);
    if (*new_mode != GAME_MODE_REVENGE)
        state->round.current_ducks_index += state->title.selected_game + 1;
    if (state->round.current_ducks_index >= MY_ARRAY_SIZE(state->round.ducks) &&
        *new_mode != GAME_MODE_REVENGE)
        *new_mode = GAME_MODE_END_ROUND;
}

void game_exit_mode(struct game *self, enum game_mode *new_mode)
{
    if (self->state.mode == GAME_MODE_TITLE)
        game_exit_title(self);
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
        if (*new_mode != GAME_MODE_SESSION_FLY_AWAY)
            game_exit_session(&self->resources.sounds);
    if (self->state.mode == GAME_MODE_END_SESSION ||
        self->state.mode == GAME_MODE_REVENGE)
        game_exit_end_session_or_revenge(&self->state, new_mode,
            &self->resources.sounds);
}
