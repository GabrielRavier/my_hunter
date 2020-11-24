/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_sprites
*/

#pragma once

#include <SFML/Graphics/Types.h>

// This is the sprites we display
struct game_sprites {
    sfSprite *title_background;
    sfSprite *title_cursor;
    sfSprite *gameplay_background;
    sfSprite *fly_away;
    sfSprite *perfect_box;
    sfSprite *text_box;
    sfSprite *dog;
};
