/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_handle_key
*/

#pragma once

#include "../../game.h"
#include <SFML/Window/Keyboard.h>

void game_handle_key(struct game *self, sfKeyCode key_code);
