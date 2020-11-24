/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_center_text_box_text
*/

#include "center_text_box_text.h"
#include <SFML/Graphics/Text.h>
#include <SFML/System/Vector2.h>

void game_center_text_box_text(sfText *self)
{
    sfText_setPosition(self, (sfVector2f){109, 64 -
        (int)(sfText_getGlobalBounds(self).height / 2)});
}
