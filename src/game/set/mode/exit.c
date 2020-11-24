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

static void game_exit_title(struct game *self)
{
    game_set_current_round(&self->state.current_round, 0);
    game_set_current_score(&self->state.scores, 0);
    sfSprite_setTextureRect(self->resources.sprites.gameplay_background,
        (sfIntRect){0, 8, 255, 224});
}

static void game_exit_session(struct game_sounds *sounds)
{
    sfSound_stop(sounds->duck.sf_sound);
    sfSound_stop(sounds->flying.sf_sound);
}

static void game_exit_end_session(struct game_state *state,
    enum game_mode *new_mode)
{
    state->round.current_ducks_index += state->title.selected_game + 1;
    if (state->round.current_ducks_index >= MY_ARRAY_SIZE(state->round.ducks))
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
    if (self->state.mode == GAME_MODE_END_SESSION)
        game_exit_end_session(&self->state, new_mode);
}
