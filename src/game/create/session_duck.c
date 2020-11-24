/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_session_duck
*/

#include "internal.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>

bool make_session_duck(struct session_duck *self, struct game *game)
{
    if (!make_sprite(&self->sprite,
        game->resources.textures.ducks, false))
        return (false);
    sfSprite_setTextureRect(self->sprite,
        (sfIntRect){0, 0, 0, 0});
    self->state = DUCK_STATE_INACTIVE;
    self->draw_shoot_rectangle = false;
    self->frames_since_state_change = 0;
    if (!make_nes_text(&self->score_text,
        game->resources.nes_font))
        return (false);
    sfText_setColor(self->score_text,
        sfColor_fromRGB(252, 252, 252));
    sfText_setPosition(self->score_text,
        (sfVector2f){1000, 1000});
    return (true);
}
