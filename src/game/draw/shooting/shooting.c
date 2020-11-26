/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_shooting
*/

#include "../shooting.h"
#include "duck_frame.h"
#include <SFML/Graphics/RenderWindow.h>

void game_draw_shooting(struct game *self)
{
    if (self->state.session.clear_screen_for_shoot) {
        self->state.session.clear_screen_for_shoot = false;
        sfRenderWindow_clear(self->window, sfBlack);
    } else
        game_draw_shooting_duck_frame(self);
}
