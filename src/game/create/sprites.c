/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_sprites
*/

#include "internal.h"
#include <SFML/Graphics/Sprite.h>

static bool make_sprites_part2(struct game_sprites *self,
    const struct game_textures *textures)
{
    if (!make_sprite(&self->text_box, textures->ducks, false))
        return (false);
    sfSprite_setTextureRect(self->text_box,
        (sfIntRect){184, 127, 233 - 184, 160 - 127});
    sfSprite_setPosition(self->text_box, (sfVector2f){104, 48});
    if (!make_sprite(&self->dog, textures->dog, false))
        return (false);
    sfSprite_setTextureRect(self->dog, (sfIntRect){0, 0, 0, 0});
    if (!make_sprite(&self->perfect_box, textures->ducks, false))
        return (false);
    sfSprite_setTextureRect(self->perfect_box,
        (sfIntRect){239, 127, 312 - 239, 160 - 127});
    return (true);
}

bool make_sprites(struct game_sprites *self,
    const struct game_textures *textures)
{
    if (!make_sprite(&self->title_background, textures->title,
        false))
        return (false);
    if (!make_sprite(&self->title_cursor, textures->cursor, true))
        return (false);
    if (!make_sprite(&self->gameplay_background,
        textures->background, false))
        return (false);
    sfSprite_setTextureRect(self->gameplay_background,
        (sfIntRect){0, 8, 255, 224});
    if (!make_sprite(&self->fly_away, textures->ducks, false))
        return (false);
    sfSprite_setTextureRect(self->fly_away,
        (sfIntRect){54, 129, 127 - 54, 146 - 129});
    sfSprite_setPosition(self->fly_away, (sfVector2f){96, 56});
    return (make_sprites_part2(self, textures));
}
