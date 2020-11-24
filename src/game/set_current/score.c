/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_set_current_score
*/

#include "score.h"
#include "../../top_score.h"
#include "../../text_utils.h"
#include <SFML/Graphics/Text.h>
#include <inttypes.h>

void game_set_current_score(struct game_scores *self, int new_score)
{
    self->current_as_int = new_score;
    if (self->current_as_int > 999999)
        self->current_as_int = 0;
    if (self->current_as_int > self->top_as_int)
        set_top_score(self->current_as_int);
    text_set_printf(self->current_as_text, "%06" PRId32, self->current_as_int);
    sfText_setColor(self->current_as_text, sfColor_fromRGB(252, 252, 252));
    sfText_setPosition(self->current_as_text, (sfVector2f){192, 199});
}
