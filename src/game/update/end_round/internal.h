/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_update_end_round internal methods
*/

#pragma once

#include "../../../game.h"

void game_update_end_round_after_sorting(struct game *self);
void game_update_end_round_before_sorting(struct game *self);
void game_update_end_round_game_over(struct game *self);
void game_update_end_round_won(struct game *self, size_t killed_ducks);
