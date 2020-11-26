/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements session_duck_update
*/

#include "../update.h"
#include "internal.h"
#include <SFML/Graphics/Sprite.h>

void session_duck_update(struct session_duck *self, struct game *game)
{
    const sfVector2f current_position = sfSprite_getPosition(self->sprite);

    ++self->frames_since_state_change;
    if (self->state == DUCK_STATE_FLYING)
        session_duck_update_flying(self, game, current_position);
    if (self->state == DUCK_STATE_FALLING)
        session_duck_update_falling(self, game, current_position);
}
