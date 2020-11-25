/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_update_do methods
*/

#pragma once

#include "../../game.h"

void game_update_do_sounds_stop_if_no_ducks_flying(struct game *self);
void game_update_do_change_to_end_session_if_all_ducks_dead(
    struct game *self);
