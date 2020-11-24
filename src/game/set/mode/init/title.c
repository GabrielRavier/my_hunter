/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_title
*/

#include "internal.h"
#include "../../../change_music.h"

void game_init_title(struct game_state *state)
{
    game_change_music(&state->current_music, "assets/title.ogg");
    state->text_box.should_draw = false;
    state->title.selected_game = 0;
}
