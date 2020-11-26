/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements round_duck_update
*/

#include "update.h"
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Sprite.h>

static const sfIntRect RECT_LIVING = {96, 209, 7, 7};
static const sfIntRect RECT_DEAD = {96, 241, 7, 7};
static const sfIntRect BLACK_RECT = {96 - 7, 209, 7, 7};
static const int FIRST_ROUND_DUCK_X = 96;
static const int X_BETWEEN_ROUND_DUCKS = 8;
static const int ROUND_DUCK_Y = 201;
static const uintmax_t FRAMES_PER_CHANGE_WHILE_FLASHING = 16;

void round_duck_update(struct round_duck *self,
    struct game_state *game_state, size_t id)
{
    sfIntRect final_rect;

    sfSprite_setPosition(self->sprite,
        (sfVector2f){FIRST_ROUND_DUCK_X + (id * X_BETWEEN_ROUND_DUCKS),
            ROUND_DUCK_Y});
    final_rect = (self->state == ROUND_DUCK_STATE_DEAD) ? RECT_DEAD :
        (self->state == ROUND_DUCK_STATE_LIVES ||
        ((self->state == ROUND_DUCK_STATE_FLYING) &&
        ((game_state->frames_since_mode_begin %
            (FRAMES_PER_CHANGE_WHILE_FLASHING * 2)) <
            FRAMES_PER_CHANGE_WHILE_FLASHING))) ? RECT_LIVING : BLACK_RECT;
    if (self->state == ROUND_DUCK_STATE_END_ROUND_FLASHING)
        final_rect = ((game_state->frames_since_mode_begin %
            (FRAMES_PER_CHANGE_WHILE_FLASHING * 2)) <
                FRAMES_PER_CHANGE_WHILE_FLASHING) ? RECT_LIVING : RECT_DEAD;
    sfSprite_setTextureRect(self->sprite, final_rect);
}
