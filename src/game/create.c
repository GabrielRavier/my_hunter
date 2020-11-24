/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_create
*/

#include "create.h"
#include "create/internal.h"
#include <SFML/Graphics/Font.h>

// We can't disable font anti-aliasing. Oh well, I guess the game will only be
// pretty on NES resolution, then ¯\_(ツ)_/¯
bool game_create(struct game *self, bool is_original_resolution)
{
    if (!make_window(&self->window, is_original_resolution))
        return (false);
    self->resources.nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (self->resources.nes_font == NULL)
        return (false);
    if (!make_textures(&self->resources.textures))
        return (false);
    if (!make_sprites(&self->resources.sprites, &self->resources.textures))
        return (false);
    if (!make_sounds(&self->resources.sounds))
        return (false);
    if (!make_game_state(&self->state, self))
        return (false);
    return (true);
}
