/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements dog methods
*/

#include "dog.h"
#include <SFML/Graphics/Sprite.h>
#include <SFML/System/Vector2.h>

void dog_update_walking(sfSprite *self)
{
    sfVector2f dog_position = sfSprite_getPosition(self);
    sfIntRect dog_rect = sfSprite_getTextureRect(self);

    dog_position.x += 2;
    if (dog_rect.top != 13)
        dog_rect = (sfIntRect){-1, 13, 56, 56 - 13};
    dog_rect.left += 56;
    if (dog_rect.left == ((56 * 4) - 1))
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
    int index = frames_since_mode_begin - 368;

    sfSprite_setPosition(self, (sfVector2f){sfSprite_getPosition(self).x +
        (index < 42 ? table_x[index] : 0), sfSprite_getPosition(self).y -
        (index < 51 ? table_y[index] : 0)});
}
