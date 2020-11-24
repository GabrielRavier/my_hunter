/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_handle_key
*/

#include "key.h"
#include "../set/mode.h"

void game_handle_key(struct game *self, sfKeyCode key_code)
{
    if (self->state.mode == GAME_MODE_TITLE) {
        if (key_code == sfKeyV)
            if (++self->state.title.selected_game > 1)
                self->state.title.selected_game = 0;
        if (key_code == sfKeyC)
            game_set_mode(self, GAME_MODE_START_ROUND);
    }
}
