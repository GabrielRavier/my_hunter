/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_shoot_frame
*/

#include "shoot_frame.h"
#include "my/assert.h"
#include "my/macros.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>

static void do_session_duck(sfRenderWindow *window, struct session_duck *duck,
    sfRectangleShape *duck_white_rectangle)
{
    sfFloatRect duck_bounds;

    if (duck->draw_shoot_rectangle) {
        duck->draw_shoot_rectangle = false;
        duck_bounds = sfSprite_getGlobalBounds(duck->sprite);
        sfRenderWindow_clear(window, sfBlack);
        sfRectangleShape_setPosition(duck_white_rectangle,
            (sfVector2f){duck_bounds.left, duck_bounds.top});
        sfRectangleShape_setSize(duck_white_rectangle,
            (sfVector2f){duck_bounds.width, duck_bounds.height});
        sfRenderWindow_drawRectangleShape(window, duck_white_rectangle, NULL);
    }
}

void game_draw_shoot_frame(struct game *self)
{
    sfRectangleShape *duck_white_rectangle = sfRectangleShape_create();

    MY_ASSERT(duck_white_rectangle);
    sfRectangleShape_setFillColor(duck_white_rectangle, sfWhite);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        do_session_duck(self->window, &self->state.session.ducks[i],
            duck_white_rectangle);
    sfRectangleShape_destroy(duck_white_rectangle);
}
