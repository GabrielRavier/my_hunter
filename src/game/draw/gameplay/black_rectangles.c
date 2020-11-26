/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_draw_gameplay_black_rectangles
*/

#include "black_rectangles.h"
#include "my/assert.h"
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Types.h>

static const sfVector2f SHOTS_LEFT_POSITION = {46, 200};
static const sfVector2f BLINKING_SHOT_INDICATOR_POSITION = {24, 208};
static const sfVector2f BLINKING_SHOT_INDICATOR_SIZE = {24, 8};
static const int X_PER_MISSING_SHOT = 8;
static const int SHOTS_LEFT_RECT_HEIGHT = 8;
static const uintmax_t FRAMES_PER_SHOT_INDICATOR_BLINK = 8;

static void current_round_rectangle(struct game *self,
    sfRectangleShape *black_rectangle)
{
    sfRectangleShape_setPosition(black_rectangle, (sfVector2f){
        sfText_getPosition(self->state.current_round.as_text).x,
        sfText_getPosition(self->state.current_round.as_text).y + 1});
    sfRectangleShape_setSize(black_rectangle, (sfVector2f){
        sfText_getGlobalBounds(self->state.current_round.as_text).width + 2,
        sfText_getGlobalBounds(self->state.current_round.as_text).height + 1});
    sfRenderWindow_drawRectangleShape(self->window, black_rectangle, NULL);
}

static void shots_left_rectangle(struct game *self, sfRectangleShape *black_rectangle)
{
    sfRectangleShape_setPosition(black_rectangle, SHOTS_LEFT_POSITION);
    sfRectangleShape_setSize(black_rectangle,
        (sfVector2f){-((3 - self->state.session.shots_left) *
            X_PER_MISSING_SHOT), SHOTS_LEFT_RECT_HEIGHT});
    sfRenderWindow_drawRectangleShape(self->window, black_rectangle, NULL);
}

static void blinking_shot_indicator_rectangle(struct game *self,
    sfRectangleShape *black_rectangle)
{
    sfRectangleShape_setPosition(black_rectangle,
        BLINKING_SHOT_INDICATOR_POSITION);
    sfRectangleShape_setSize(black_rectangle, BLINKING_SHOT_INDICATOR_SIZE);
    sfRenderWindow_drawRectangleShape(self->window, black_rectangle, NULL);
}

void game_draw_gameplay_black_rectangles(struct game *self)
{
    sfRectangleShape *black_rectangle = sfRectangleShape_create();

    MY_ASSERT(black_rectangle != NULL);
    sfRectangleShape_setFillColor(black_rectangle, sfBlack);
    current_round_rectangle(self, black_rectangle);
    if (self->state.mode != GAME_MODE_START_ROUND)
        shots_left_rectangle(self, black_rectangle);
    if ((self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY) &&
        (self->state.session.shots_left == 0) &&
        ((self->state.frames_since_mode_begin %
        (FRAMES_PER_SHOT_INDICATOR_BLINK * 2)) <
        FRAMES_PER_SHOT_INDICATOR_BLINK))
        blinking_shot_indicator_rectangle(self, black_rectangle);
    sfRectangleShape_destroy(black_rectangle);
}
