/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_text_box
*/

#include "text_box.h"
#include <SFML/Graphics/RenderWindow.h>

void game_draw_text_box(struct game *self)
{
    sfRenderWindow_drawSprite(self->window, self->resources.sprites.text_box,
        NULL);
    sfRenderWindow_drawText(self->window, self->state.text_box.text, NULL);
}
