/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares session_duck_update methods
*/

#pragma once

#include "../../session_duck.h"
#include "../../../game.h"
#include <SFML/System/Vector2.h>

void session_duck_update_falling(struct session_duck *self, struct game *game,
    sfVector2f current_position);
void session_duck_update_flying(struct session_duck *self, struct game *game,
    sfVector2f current_position);
