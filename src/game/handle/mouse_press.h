/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_handle_mouse_press
*/

#pragma once

#include "../../game.h"
#include <SFML/Window/Event.h>

void game_handle_mouse_press(struct game *self,
    const sfMouseButtonEvent *mouse_button);
