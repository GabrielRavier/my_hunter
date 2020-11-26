/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements session_duck_update_falling
*/

#include "internal.h"
#include "../set.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>
#include <math.h>

static const sfIntRect RECTS_JUST_GOT_SHOT[3] = {{226, 12, 257 - 226, 41 - 12},
    {224, 52, 255 - 224, 81 - 52}, {222, 95, 253 - 222, 124 - 95}};
static const sfIntRect RECTS_FALLING[3] = {{267, 10, 285 - 267, 41 - 10},
    {264, 51, 282 - 264, 82 - 51}, {263, 93, 281 - 263, 124 - 93}};
static const int FRAME_START_FALLING = 22;
static const int FRAMES_PER_SIDE_CHANGE = 5;
static const sfVector2f POSITION_OUT_OF_SCREEN = {1000, 1000};
static const int DUCK_DEAD_Y = 184 - (119 - 89);
static const int FRAMES_SCORE_STAYS_VISIBLE = 45;

static void do_really_falling(struct session_duck *self, struct game *game,
    sfVector2f current_position)
{
    sfIntRect final_rect;

    if (self->frames_since_state_change >
        (FRAME_START_FALLING + FRAMES_SCORE_STAYS_VISIBLE))
        sfText_setPosition(self->score_text, POSITION_OUT_OF_SCREEN);
    if ((self->frames_since_state_change % 2) == 0)
        ++current_position.x;
    else
        --current_position.x;
    current_position.y += 2;
    sfSprite_setPosition(self->sprite, current_position);
    if (current_position.y > DUCK_DEAD_Y)
        session_duck_set_state(self, game, DUCK_STATE_DEAD);
    final_rect = RECTS_FALLING[self->color];
    if ((self->frames_since_state_change % (FRAMES_PER_SIDE_CHANGE * 2)) <
        FRAMES_PER_SIDE_CHANGE)
        final_rect = (sfIntRect){final_rect.left + final_rect.width,
            final_rect.top, -final_rect.width, final_rect.height};
    sfSprite_setTextureRect(self->sprite, final_rect);
}

void session_duck_update_falling(struct session_duck *self, struct game *game,
    sfVector2f current_position)
{
    sfIntRect final_rect;

    if (self->frames_since_state_change < FRAME_START_FALLING) {
        final_rect = RECTS_JUST_GOT_SHOT[self->color];
        if (cosf(self->angle) < 0)
            final_rect = (sfIntRect){final_rect.left + final_rect.width,
                final_rect.top, -final_rect.width, final_rect.height};
        sfSprite_setTextureRect(self->sprite, final_rect);
    } else if (self->frames_since_state_change == FRAME_START_FALLING) {
        sfSound_play(game->resources.sounds.duck_falling.sf_sound);
        session_duck_set_text_position(self);
    } else
        do_really_falling(self, game, current_position);
}
