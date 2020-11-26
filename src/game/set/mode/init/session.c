/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_session
*/

#include "internal.h"
#include "my/macros.h"
#include "../../../get_duck_speed.h"
#include "../../../../random.h"
#include <SFML/Audio/Music.h>
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Sprite.h>
#include <math.h>

static const int MAX_INITIAL_DUCK_X_POSITION = 256 - (33 - 6);
static const int INITIAL_DUCK_Y_POSITION = 184 - (119 - 89);
static const int INITIAL_AIMED_Y_POSITION = 120;
static const int MIN_INITIAL_AIMED_X_POSITION = 25;
static const int MAX_INITIAL_AIMED_X_POSITION = 230;

static void init_session_duck(struct session_duck *self, struct game *game)
{
    if (self->state != DUCK_STATE_INACTIVE) {
        sfSprite_setPosition(self->sprite, (sfVector2f){
            random_int_between(0, MAX_INITIAL_DUCK_X_POSITION),
            INITIAL_DUCK_Y_POSITION});
        self->color = random_int_between(0, 2);
        self->speed = game_get_duck_speed(&game->state, self);
        self->angle = atan2f(INITIAL_AIMED_Y_POSITION -
            sfSprite_getPosition(self->sprite).y,
            random_int_between(MIN_INITIAL_AIMED_X_POSITION,
                MAX_INITIAL_AIMED_X_POSITION) -
                   sfSprite_getPosition(self->sprite).x);
    }
}

void game_init_session(struct game *self)
{
    self->state.text_box.should_draw = false;
    sfMusic_destroy(self->state.current_music);
    self->state.current_music = NULL;
    self->state.session.ducks[0].state = DUCK_STATE_FLYING;
    self->state.round.ducks[self->state.round.current_ducks_index].state =
        ROUND_DUCK_STATE_FLYING;
    if (self->state.title.selected_game == 1) {
        self->state.round.ducks[self->state.round.current_ducks_index + 1]
            .state = ROUND_DUCK_STATE_FLYING;
        self->state.session.ducks[1].state = DUCK_STATE_FLYING;
    }
    else
        self->state.session.ducks[1].state = DUCK_STATE_INACTIVE;
    sfSound_play(self->resources.sounds.duck.sf_sound);
    sfSound_play(self->resources.sounds.flying.sf_sound);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        init_session_duck(&self->state.session.ducks[i], self);
    self->state.session.shots_left = 3;
}
