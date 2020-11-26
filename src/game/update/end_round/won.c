/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_end_round_won
*/

#include "internal.h"
#include "../../set/mode.h"
#include "../../set/current/score.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Sprite.h>
#include "my/macros.h"

static const int PERFECT_SCORE_REWARD = 10000;
static const sfVector2f PERFECT_SPRITE_POSITION = {96, 48};
static const int FRAME_START_MUSIC_AND_FLASHING_DUCKS = 16;
static const int FRAME_DO_PERFECT_OR_FINISH = 280;
static const int FRAME_FINISH_AFTER_PERFECT = 450;

static void do_perfect_or_finish(struct game *self, size_t killed_ducks)
{
    if (killed_ducks < MY_ARRAY_SIZE(self->state.round.ducks))
        game_set_mode(self, GAME_MODE_START_ROUND);
    else {
        game_set_current_score(&self->state.scores,
            self->state.scores.current_as_int + PERFECT_SCORE_REWARD);
        sfSound_play(self->resources.sounds.perfect.sf_sound);
        sfSprite_setPosition(self->resources.sprites.perfect_box,
            PERFECT_SPRITE_POSITION);
        for (size_t i = 0; i < killed_ducks; ++i)
            self->state.round.ducks[i].state = ROUND_DUCK_STATE_DEAD;
    }
}

void game_update_end_round_won(struct game *self, size_t killed_ducks)
{
    if (((uintmax_t) self->state.round.frame_finished_sorting +
            FRAME_START_MUSIC_AND_FLASHING_DUCKS) ==
        self->state.frames_since_mode_begin) {
        sfSound_play(self->resources.sounds.end_round_success.sf_sound);
        for (size_t i = 0; i < killed_ducks; ++i)
            self->state.round.ducks[i].state =
                ROUND_DUCK_STATE_END_ROUND_FLASHING;
    }
    if (((uintmax_t) self->state.round.frame_finished_sorting +
            FRAME_DO_PERFECT_OR_FINISH) == self->state.frames_since_mode_begin)
        do_perfect_or_finish(self, killed_ducks);
    if (((uintmax_t) self->state.round.frame_finished_sorting +
            FRAME_FINISH_AFTER_PERFECT) == self->state.frames_since_mode_begin)
        game_set_mode(self, GAME_MODE_START_ROUND);
}
