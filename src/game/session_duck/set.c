/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements session_duck_set methods
*/

#include "set.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>

static const sfVector2f POSITION_OUT_OF_SCREEN = {1000, 1000};

void session_duck_set_state(struct session_duck *self, struct game *game,
    enum duck_state state)
{
    self->state = state;
    self->frames_since_state_change = 0;
    if (self->state == DUCK_STATE_FALLING)
        game->state.round.ducks[(self - game->state.session.ducks) +
            game->state.round.current_ducks_index].state =
                ROUND_DUCK_STATE_DEAD;
    if (self->state == DUCK_STATE_DEAD) {
        sfText_setPosition(self->score_text, POSITION_OUT_OF_SCREEN);
        sfSound_play(game->resources.sounds.duck_thud.sf_sound);
        sfSound_stop(game->resources.sounds.duck_falling.sf_sound);
        game->state.session.last_duck_fall_x_position =
            sfSprite_getPosition(self->sprite).x;
    }
    if (self->state == DUCK_STATE_INACTIVE)
        game->state.round.ducks[(self - game->state.session.ducks) +
            game->state.round.current_ducks_index].state = ROUND_DUCK_STATE_LIVES;
}

void session_duck_set_text_position(struct session_duck *self)
{
    const sfFloatRect text_bounds = sfText_getGlobalBounds(self->score_text);
    const sfFloatRect duck_bounds = sfSprite_getGlobalBounds(self->sprite);

    sfText_setPosition(self->score_text, (sfVector2f){
        (int)(duck_bounds.left +
             ((duck_bounds.width - text_bounds.width) / 2)),
        (int)(duck_bounds.top +
             ((duck_bounds.height - text_bounds.height) / 2))});
}

