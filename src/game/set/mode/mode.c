/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_set_mode
*/

#include "../mode.h"
#include "exit.h"
#include "init.h"

void game_set_mode(struct game *self, enum game_mode new_mode)
{
    game_exit_mode(self, &new_mode);
    self->state.mode = new_mode;
    self->state.frames_since_mode_begin = 0;
    game_init_mode(self);
}
