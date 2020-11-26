/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_title
*/

#include "title.h"
#include <SFML/Graphics/RenderWindow.h>

void game_draw_title(struct game *self)
{
    sfRenderWindow_drawSprite(self->window,
        self->resources.sprites.title_background, NULL);
    if (self->state.frames_since_mode_begin >= 1) {
        sfRenderWindow_drawText(self->window, self->state.scores.top_as_text,
            NULL);
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.title_cursor, NULL);
    }
}
