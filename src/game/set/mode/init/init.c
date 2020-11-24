/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_mode
*/

#include "../init.h"
#include "internal.h"

void game_init_mode(struct game *self)
{
    if (self->state.mode == GAME_MODE_TITLE)
        game_init_title(&self->state);
    if (self->state.mode == GAME_MODE_START_ROUND)
        game_init_start_round(self);
    if (self->state.mode == GAME_MODE_SESSION)
        game_init_session(self);
    if (self->state.mode == GAME_MODE_END_ROUND)
        game_init_end_round(self);
}
