/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares internal stuff for init
*/

#pragma once

#include "../../../state.h"
#include "../../../../game.h"

void game_init_title(struct game_state *state);
void game_init_start_round(struct game *self);
void game_init_session(struct game *self);
void game_init_end_round(struct game *self);
