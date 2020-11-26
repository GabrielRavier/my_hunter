/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_end_round
*/

#include "internal.h"
#include <SFML/Graphics/Sprite.h>

static const sfVector2f POSITION_OUT_OF_SCREEN = {1000, 1000};

void game_init_end_round(struct game *self)
{
    self->state.round.frame_finished_sorting = -1;
    sfSprite_setPosition(self->resources.sprites.dog, POSITION_OUT_OF_SCREEN);
    sfSprite_setPosition(self->resources.sprites.perfect_box,
        POSITION_OUT_OF_SCREEN);
}
