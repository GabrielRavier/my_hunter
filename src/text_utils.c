/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements text_utils.h
*/

#include "text_utils.h"
#include "my/stdio.h"
#include "my/assert.h"
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Texture.h>
#include <stdlib.h>
#include <stdarg.h>

void text_set_printf(sfText *text, const char *format, ...)
{
    char *formatted_string;
    va_list arguments;

    va_start(arguments, format);
    MY_ASSERT(my_vasprintf(&formatted_string, format, arguments) >= 0);
    va_end(arguments);
    sfText_setString(text, formatted_string);
    free(formatted_string);
}

sfText *make_nes_text(sfFont *nes_font)
{
    sfText *result = sfText_create();
    static const int NES_CHARACTER_SIZE = 8;

    MY_ASSERT(result);
    sfText_setFont(result, nes_font);
    sfText_setCharacterSize(result, NES_CHARACTER_SIZE);
    return result;
}
