/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_session_state
*/

#pragma once

#include "session_duck.h"
#include <stdbool.h>

// This is the state for a session
// clear_screen_for_shoot determines whether we just shot and thus need to clear
// the screen
// last_duck_fall_x_position is the x position of the latest place a duck fell
// at
// shots_left is the amount of ammunition the player has left
struct game_session_state {
    struct session_duck ducks[2];
    bool clear_screen_for_shoot;
    float last_duck_fall_x_position;
    int shots_left;
};
