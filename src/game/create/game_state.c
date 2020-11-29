/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_game_state
*/

#include "internal.h"
#include "my/macros.h"
#include <SFML/Graphics/Text.h>

static bool make_game_revenge_state(struct game_revenge_state *self,
    sfFont *nes_font)
{
    const sfColor text_color = sfColor_fromRGB(252, 252, 252);

    if (!make_nes_text(&self->text, nes_font))
        return (false);
    sfText_setString(self->text, "          OUCH !!\n"
        "SHOOT THE DUCKS, NOT ME !!");
    sfText_setColor(self->text, text_color);
    return (true);
}

bool make_game_state(struct game_state *self, struct game *game)
{
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->round.ducks); ++i)
        if (!make_sprite(&self->round.ducks[i].sprite,
            game->resources.textures.background, false))
            return (false);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->session.ducks); ++i)
        if (!make_session_duck(&self->session.ducks[i], game))
            return (false);
    self->session.clear_screen_for_shoot = false;
    if (!make_scores(&self->scores, game->resources.nes_font))
        return (false);
    if (!make_nes_text(&self->text_box.text, game->resources.nes_font))
        return (false);
    if (!make_nes_text(&self->current_round.as_text, game->resources.nes_font))
        return (false);
    if (!make_game_revenge_state(&self->revenge, game->resources.nes_font))
        return (false);
    self->current_music = NULL;
    self->mode = GAME_MODE_NONE;
    return (true);
}
