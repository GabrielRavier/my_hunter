/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw
*/

#include "../draw.h"
#include "title.h"
#include "gameplay.h"
#include "text_box.h"
#include "shooting.h"
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/RenderWindow.h>

void game_draw(struct game *self)
{
    sfRenderWindow_clear(self->window, sfBlack);
    if (self->state.mode == GAME_MODE_TITLE)
        game_draw_title(self);
    if (self->state.mode == GAME_MODE_START_ROUND ||
        self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY ||
        self->state.mode == GAME_MODE_END_SESSION ||
        self->state.mode == GAME_MODE_REVENGE ||
        self->state.mode == GAME_MODE_END_ROUND)
        game_draw_gameplay(self);
    if (self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.fly_away, NULL);
    if (self->state.text_box.should_draw)
        game_draw_text_box(self);
    if (self->state.mode == GAME_MODE_END_ROUND)
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.perfect_box, NULL);
    game_draw_shooting(self);
}
