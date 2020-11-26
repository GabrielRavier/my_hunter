/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_start_round
*/

#include "internal.h"
#include "../../current/round.h"
#include "../../../change_music.h"
#include "../../../center_text_box_text.h"
#include "../../../../text_utils.h"
#include "my/macros.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>

static const sfVector2f DOG_START_ROUND_POSITION = {2, 136};

static void init_round_duck(struct round_duck *self)
{
    sfSprite_setTextureRect(self->sprite, (sfIntRect){0, 0, 0, 0});
    self->state = ROUND_DUCK_STATE_LIVES;
}

void game_init_start_round(struct game *self)
{
    const sfColor text_box_text_color = sfColor_fromRGB(252, 252, 252);

    game_change_music(&self->state.current_music, "assets/start_round.ogg");
    game_set_current_round(&self->state.current_round,
        self->state.current_round.as_int + 1);
    self->state.text_box.should_draw = true;
    text_set_printf(self->state.text_box.text, "ROUND\n\n %2d",
        self->state.current_round.as_int);
    sfText_setColor(self->state.text_box.text, text_box_text_color);
    game_center_text_box_text(self->state.text_box.text);
    sfSprite_setPosition(self->resources.sprites.dog, DOG_START_ROUND_POSITION);
    sfSprite_setTextureRect(self->resources.sprites.dog,
        (sfIntRect){0, 0, 0, 0});
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
        init_round_duck(&self->state.round.ducks[i]);
    self->state.round.current_ducks_index = 0;
}
