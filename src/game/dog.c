/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements dog methods
*/

#include "dog.h"
#include "my/macros.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/System/Vector2.h>
#include <stddef.h>

const int WALKING_DOG_TOP = 13;
const int WALKING_DOG_WIDTH = 56;
const int WALKING_DOG_PRE_FIRST_LEFT = -1;
const sfIntRect WALKING_DOG_PRE_FIRST_RECT = {WALKING_DOG_PRE_FIRST_LEFT,
    WALKING_DOG_TOP, WALKING_DOG_WIDTH, 56 - 13};
const uintmax_t FRAME_START_FLYING = 368;

void dog_update_walking(sfSprite *self)
{
    sfVector2f dog_position = sfSprite_getPosition(self);
    sfIntRect dog_rect = sfSprite_getTextureRect(self);

    dog_position.x += 2;
    if (dog_rect.top != WALKING_DOG_TOP)
        dog_rect = WALKING_DOG_PRE_FIRST_RECT;
    dog_rect.left += WALKING_DOG_WIDTH;
    if (dog_rect.left == ((WALKING_DOG_WIDTH * 4) + WALKING_DOG_PRE_FIRST_LEFT))
        dog_rect.left = -1;
    sfSprite_setPosition(self, dog_position);
    sfSprite_setTextureRect(self, dog_rect);
}

void dog_update_flying(sfSprite *self,
    uintmax_t frames_since_mode_begin)
{
    static const int table_x[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1};
    static const int table_y[51] = {3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -1, 0, -1, -1, -2,
        -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
    uintmax_t index = frames_since_mode_begin - FRAME_START_FLYING;

    sfSprite_setPosition(self, (sfVector2f){sfSprite_getPosition(self).x +
            (index < MY_ARRAY_SIZE(table_x) ? table_x[index] : 0),
        sfSprite_getPosition(self).y -
            (index < MY_ARRAY_SIZE(table_y) ? table_y[index] : 0)});
}
