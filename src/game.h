/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include "game/resources.h"
#include "game/state.h"

// This is the main struct for the entire game
struct game {
    sfRenderWindow *window;
    struct game_resources resources;
    struct game_state state;
};
