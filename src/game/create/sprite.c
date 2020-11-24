/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_sprite
*/

#include "internal.h"
#include <SFML/Graphics/Sprite.h>

bool make_sprite(sfSprite **sprite, sfTexture *texture, bool reset_rect)
{
    *sprite = sfSprite_create();
    if (*sprite == NULL)
        return (false);
    sfSprite_setTexture(*sprite, texture, reset_rect);
    return (true);
}
