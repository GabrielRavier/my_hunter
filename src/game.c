/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "game.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Texture.h>

static void game_draw(struct game *self)
{
    sfRenderWindow_drawSprite(self->window, self->title_background_sprite,
        NULL);
}

bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 240, 32},
        "Duck Hunt but it's done with CSFML", sfResize | sfClose, NULL);
    if (!self->window)
        return (false);
    self->title_background_texture = sfTexture_createFromFile(
        "assets/title.png", NULL);
    if (!self->title_background_texture)
        return (false);
    self->title_background_sprite = sfSprite_create();
    if (!self->title_background_sprite)
        return (false);
    sfSprite_setTexture(self->title_background_sprite, self->title_background_texture, true);
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
        game_draw(self);
        sfRenderWindow_display(self->window);
    }
}

void game_destroy(struct game *self)
{
    sfSprite_destroy(self->title_background_sprite);
    sfTexture_destroy(self->title_background_texture);
    sfRenderWindow_destroy(self->window);
}
