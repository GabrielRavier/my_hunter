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

static const sfIntRect rects_just_got_shot[3] = {{226, 12, 257 - 226, 41 - 12},
    {224, 52, 255 - 224, 81 - 52}, {222, 95, 253 - 222, 124 - 95}};

static const sfIntRect rects_falling[3] = {{267, 10, 285 - 267, 41 - 10},
    {264, 51, 282 - 264, 82 - 51}, {263, 93, 281 - 263, 124 - 93}};

static void do_really_falling(struct session_duck *self, struct game *game, sfVector2f current_position)
{
    sfIntRect final_rect;

    if (self->frames_since_state_change > (22 + 45))
        sfText_setPosition(self->score_text, (sfVector2f){1000, 1000});
    if ((self->frames_since_state_change % 2) == 0)
        ++current_position.x;
    else
        --current_position.x;
    current_position.y += 2;
    sfSprite_setPosition(self->sprite, current_position);
    if (current_position.y > 184 - (119 - 89))
        session_duck_set_state(self, game, DUCK_STATE_DEAD);
    final_rect = rects_falling[self->color];
    if ((self->frames_since_state_change % 10) < 5)
        final_rect = (sfIntRect){final_rect.left + final_rect.width,
            final_rect.top, -final_rect.width, final_rect.height};
    sfSprite_setTextureRect(self->sprite, final_rect);
}

void session_duck_update_falling(struct session_duck *self,
    struct game *game, sfVector2f current_position)
{
    sfIntRect final_rect;

    if (self->frames_since_state_change < 22) {
        final_rect = rects_just_got_shot[self->color];
        if (cosf(self->angle) < 0)
            final_rect = (sfIntRect){final_rect.left + final_rect.width,
                final_rect.top, -final_rect.width, final_rect.height};
        sfSprite_setTextureRect(self->sprite,
                                final_rect);
    } else if (self->frames_since_state_change == 22) {
        sfSound_play(game->resources.sounds.duck_falling.sf_sound);
        session_duck_set_text_position(self);
    } else
        do_really_falling(self, game, current_position);
}
