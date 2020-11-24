/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares session_duck
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include <stdbool.h>

// This is one of the ducks you shoot at
// color is one of 0 (black), 1 (red) or 2 (blue)
// draw_shoot_rectangle determines whether we should be drawing the white
// rectangle corresponding to the duck (when shooting)
// score_text is the text we draw when the duck is shot
struct session_duck {
    sfSprite *sprite;
    enum duck_state {
        DUCK_STATE_INACTIVE,
        DUCK_STATE_FLYING,
        DUCK_STATE_FALLING,
        DUCK_STATE_DEAD,
    } state;
    int frames_since_state_change;
    float angle;
    float speed;
    int color;
    bool draw_shoot_rectangle;
    sfText *score_text;
};
