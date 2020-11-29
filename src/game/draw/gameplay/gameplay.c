/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_gameplay
*/

#include "../gameplay.h"
#include "background_color.h"
#include "black_rectangles.h"
#include "my/assert.h"
#include "my/macros.h"
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Text.h>

static const uintmax_t FRAME_DOG_START_GOING_BEHIND_GAMEPLAY_BACKGROUND =
    368 + 18;
static const uintmax_t FRAME_REVENGE_WALKING_START = 8 * 2 + 4 + 15 + 10;

static void draw_session_ducks(struct game *self)
{
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        if (self->state.session.ducks[i].state != DUCK_STATE_INACTIVE)
            sfRenderWindow_drawSprite(self->window,
                self->state.session.ducks[i].sprite, NULL);
}

static void do_draw_dog_before_background(struct game *self)
{
    if ((self->state.mode == GAME_MODE_START_ROUND ||
            self->state.mode == GAME_MODE_END_SESSION ||
            self->state.mode == GAME_MODE_END_ROUND ||
            self->state.mode == GAME_MODE_REVENGE) &&
        (!(self->state.mode == GAME_MODE_START_ROUND) ||
            self->state.frames_since_mode_begin >
            FRAME_DOG_START_GOING_BEHIND_GAMEPLAY_BACKGROUND) &&
        (!(self->state.mode == GAME_MODE_REVENGE &&
            self->state.frames_since_mode_begin >=
            FRAME_REVENGE_WALKING_START)))
        sfRenderWindow_drawSprite(self->window, self->resources.sprites.dog,
            NULL);
}

static void do_draw_dog_after_background(struct game *self)
{
    if ((self->state.mode == GAME_MODE_START_ROUND &&
        (self->state.frames_since_mode_begin <=
        FRAME_DOG_START_GOING_BEHIND_GAMEPLAY_BACKGROUND)) ||
        (self->state.mode == GAME_MODE_REVENGE &&
        (self->state.frames_since_mode_begin >=
        FRAME_REVENGE_WALKING_START)))
        sfRenderWindow_drawSprite(self->window, self->resources.sprites.dog,
            NULL);
}

static void do_ducks(struct game *self)
{
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
        sfRenderWindow_drawSprite(self->window,
            self->state.round.ducks[i].sprite, NULL);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        sfRenderWindow_drawText(self->window,
            self->state.session.ducks[i].score_text, NULL);
}

void game_draw_gameplay(struct game *self)
{
    game_draw_gameplay_background_color(self);
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
        draw_session_ducks(self);
    do_draw_dog_before_background(self);
    sfRenderWindow_drawSprite(self->window,
        self->resources.sprites.gameplay_background, NULL);
    game_draw_gameplay_black_rectangles(self);
    sfRenderWindow_drawText(self->window, self->state.current_round.as_text,
        NULL);
    sfRenderWindow_drawText(self->window, self->state.scores.current_as_text,
        NULL);
    do_draw_dog_after_background(self);
    do_ducks(self);
    if (self->state.mode == GAME_MODE_REVENGE)
        sfRenderWindow_drawText(self->window, self->state.revenge.text, NULL);
}
