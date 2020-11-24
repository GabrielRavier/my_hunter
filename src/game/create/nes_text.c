/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_nes_text
*/

#include "internal.h"
#include <SFML/Graphics/Text.h>

bool make_nes_text(sfText **text, sfFont *nes_font)
{
    static const int NES_CHARACTER_SIZE = 8;

    *text = sfText_create();
    if (*text == NULL)
        return (false);
    sfText_setFont(*text, nes_font);
    sfText_setCharacterSize(*text, NES_CHARACTER_SIZE);
    return (true);
}
