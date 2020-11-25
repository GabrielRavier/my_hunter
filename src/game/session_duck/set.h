/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares session_duck_set methods
*/

#pragma once

#include "../session_duck.h"
#include "../../game.h"

void session_duck_set_state(struct session_duck *self, struct game *game,
    enum duck_state state);
void session_duck_set_text_position(struct session_duck *self);
