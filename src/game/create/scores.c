/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_scores
*/

#include "internal.h"
#include "../../top_score.h"
#include "../../text_utils.h"
#include <SFML/Graphics/Text.h>
#include <inttypes.h>

static const int TOP_SCORE_RIGHTMOST_X = 191;
static const int TOP_SCORE_Y = 183;

bool make_scores(struct game_scores *self, sfFont *nes_font)
{
    const sfColor top_score_color = sfColor_fromRGB(76, 220, 72);

    self->top_as_int = get_top_score();
    if (!make_nes_text(&self->top_as_text, nes_font))
        return (false);
    text_set_printf(self->top_as_text, "%" PRId32, self->top_as_int);
    sfText_setColor(self->top_as_text, top_score_color);
    sfText_setPosition(self->top_as_text, (sfVector2f){TOP_SCORE_RIGHTMOST_X -
        sfText_getGlobalBounds(self->top_as_text).width,
        TOP_SCORE_Y});
    if (!make_nes_text(&self->current_as_text, nes_font))
        return (false);
    return (true);
}
