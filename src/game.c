/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "game.h"

bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 240, 32},
        "Duck Hunt but it's done with CSFML", sfResize | sfClose, NULL);

    if (!self->window)
        return (false);
    return (true);
}

void game_main_loop(struct game *self)
{
    sfEvent event;

    while (sfRenderWindow_isOpen(self->window)) {
        while (sfRenderWindow_pollEvent(self->window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(self->window);
        }
        sfRenderWindow_clear(self->window, sfBlack);
        sfRenderWindow_display(self->window);
    }
}

void game_destroy(struct game *self)
{
    sfRenderWindow_destroy(self->window);
}
