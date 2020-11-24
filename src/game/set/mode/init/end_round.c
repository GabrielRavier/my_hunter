/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_end_round
*/

#include "internal.h"
#include <SFML/Graphics/Sprite.h>

void game_init_end_round(struct game *self)
{
    self->state.round.frame_finished_sorting = -1;
    sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){1000, 1000});
    sfSprite_setPosition(self->resources.sprites.perfect_box,
        (sfVector2f){1000, 1000});
}
