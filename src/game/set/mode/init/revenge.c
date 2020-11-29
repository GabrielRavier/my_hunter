/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_init_revenge
*/

#include "internal.h"
#include "../../../change_music.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>

static const int DOG_START_Y = 125;
static const sfIntRect JUST_SHOT_RECT = {144, 377, 165 - 144, 406 - 377};
static const sfVector2f POSITION_OUT_OF_SCREEN = {1000, 1000};

void game_init_revenge(struct game *self)
{
    float old_dog_x = sfSprite_getPosition(self->resources.sprites.dog).x;

    game_change_music(&self->state.current_music, "assets/revenge.ogg");
    sfSprite_setPosition(self->resources.sprites.dog,
        (sfVector2f){old_dog_x + 3, DOG_START_Y});
    sfSprite_setTextureRect(self->resources.sprites.dog, JUST_SHOT_RECT);
    sfText_setPosition(self->state.revenge.text, POSITION_OUT_OF_SCREEN);
}
