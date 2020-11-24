/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_set_current_round
*/

#include "round.h"
#include "../../text_utils.h"
#include <SFML/Graphics/Text.h>
#include <inttypes.h>

void game_set_current_round(struct game_round *self, int new_round)
{
    self->as_int = new_round;
    if (self->as_int > 99)
        self->as_int = 1;
    text_set_printf(self->as_text, "%" PRId32, self->as_int);
    sfText_setColor(self->as_text, sfColor_fromRGB(128, 208, 16));
    sfText_setPosition(self->as_text, (sfVector2f){40, 183});
}
