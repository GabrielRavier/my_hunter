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

bool make_scores(struct game_scores *self, sfFont *nes_font)
{
    self->top_as_int = get_top_score();
    if (!make_nes_text(&self->top_as_text, nes_font))
        return (false);
    text_set_printf(self->top_as_text, "%" PRId32, self->top_as_int);
    sfText_setColor(self->top_as_text, sfColor_fromRGB(76, 220, 72));
    sfText_setPosition(self->top_as_text, (sfVector2f){191 -
        sfText_getGlobalBounds(self->top_as_text).width, 183});
    if (!make_nes_text(&self->current_as_text, nes_font))
        return (false);
    return (true);
}
