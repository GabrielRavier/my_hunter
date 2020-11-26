/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_set_current_round
*/

#include "round.h"
#include "../../../text_utils.h"
#include <SFML/Graphics/Text.h>
#include <inttypes.h>

static const int MAXIMUM_ROUND = 99;
static const sfVector2f ROUND_TEXT_POSITION = {40, 183};

void game_set_current_round(struct game_round *self, int new_round)
{
    const sfColor round_text_color = sfColor_fromRGB(128, 208, 16);

    self->as_int = new_round;
    if (self->as_int > MAXIMUM_ROUND)
        self->as_int = 1;
    text_set_printf(self->as_text, "%" PRId32, self->as_int);
    sfText_setColor(self->as_text, round_text_color);
    sfText_setPosition(self->as_text, ROUND_TEXT_POSITION);
}
