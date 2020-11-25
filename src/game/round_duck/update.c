/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements round_duck_update
*/

#include "update.h"
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Sprite.h>

void round_duck_update(struct round_duck *self,
    struct game_state *game_state, size_t id)
{
    sfIntRect rect_living = {96, 209, 7, 7};
    sfIntRect rect_dead = {96, 241, 7, 7};
    sfIntRect black_rect = {96 - 7, 209, 7, 7};
    sfIntRect final_rect;

    sfSprite_setPosition(self->sprite,
        (sfVector2f){96 + (id * 8), 201});
    if (self->state == ROUND_DUCK_STATE_DEAD)
        final_rect = rect_dead;
    else if (self->state == ROUND_DUCK_STATE_LIVES ||
        ((self->state == ROUND_DUCK_STATE_FLYING) &&
        ((game_state->frames_since_mode_begin % 32) < 16)))
        final_rect = rect_living;
    else
        final_rect = black_rect;
    if (self->state == ROUND_DUCK_STATE_END_ROUND_FLASHING) {
        if ((game_state->frames_since_mode_begin % 32) < 16)
            final_rect = rect_living;
        else
            final_rect = rect_dead;
    }
    sfSprite_setTextureRect(self->sprite, final_rect);
}
