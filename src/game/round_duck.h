/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares round_duck
*/

#pragma once

#include <SFML/Graphics/Types.h>

// This is one of the ducks at the bottom of the screen, at the right of "HIT"
struct round_duck {
    enum round_duck_state {
        ROUND_DUCK_STATE_LIVES,
        ROUND_DUCK_STATE_DEAD,
        ROUND_DUCK_STATE_FLYING,
        ROUND_DUCK_STATE_END_ROUND_FLASHING,
    } state;
    sfSprite *sprite;
};
