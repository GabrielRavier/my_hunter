/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_textures
*/

#include "internal.h"
#include <SFML/Graphics/Texture.h>

static bool make_texture(sfTexture **texture, const char *filename)
{
    *texture = sfTexture_createFromFile(filename, NULL);
    return (*texture != NULL);
}

bool make_textures(struct game_textures *self)
{
    if (!make_texture(&self->title, "assets/title.png"))
        return (false);
    if (!make_texture(&self->cursor, "assets/cursor.png"))
        return (false);
    if (!make_texture(&self->background, "assets/backgrounds.png"))
        return (false);
    if (!make_texture(&self->ducks, "assets/ducks.png"))
        return (false);
    if (!make_texture(&self->dog, "assets/dog.png"))
        return (false);
    return (true);
}
