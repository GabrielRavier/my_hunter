/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_set_current_score
*/

#include "score.h"
#include "../../../top_score.h"
#include "../../../text_utils.h"
#include <SFML/Graphics/Text.h>
#include <inttypes.h>

static const int32_t MAXIMUM_SCORE = 999999;
static const sfVector2f SCORE_TEXT_POSITION = {192, 199};

void game_set_current_score(struct game_scores *self, int new_score)
{
    const sfColor score_text_color = sfColor_fromRGB(252, 252, 252);

    self->current_as_int = new_score;
    if (self->current_as_int > MAXIMUM_SCORE)
        self->current_as_int = 0;
    if (self->current_as_int > self->top_as_int)
        set_top_score(self->current_as_int);
    text_set_printf(self->current_as_text, "%06" PRId32, self->current_as_int);
    sfText_setColor(self->current_as_text, score_text_color);
    sfText_setPosition(self->current_as_text, SCORE_TEXT_POSITION);
}
