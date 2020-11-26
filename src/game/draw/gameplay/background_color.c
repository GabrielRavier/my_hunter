/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_background_color
*/

#include "background_color.h"
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/RenderWindow.h>

void game_draw_gameplay_background_color(struct game *self)
{
    const sfColor background_color =
        (self->state.mode == GAME_MODE_SESSION_FLY_AWAY) ?
        sfColor_fromRGB(252, 188, 176) : sfColor_fromRGB(60, 188, 252);

    sfRenderWindow_clear(self->window, background_color);
}
