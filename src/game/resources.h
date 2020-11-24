/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_resources
*/

#pragma once

#include "textures.h"
#include "sprites.h"
#include "sounds.h"
#include <SFML/Graphics/Types.h>

// This is all the resources we use in the game
struct game_resources {
    sfFont *nes_font;
    struct game_textures textures;
    struct game_sprites sprites;
    struct game_sounds sounds;
};
