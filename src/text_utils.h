/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declarations for text_utils.c
*/

#pragma once

#include <SFML/Graphics/Types.h>

void text_set_printf(sfText *text, const char *format, ...);
sfText *make_nes_text(sfFont *nes_font);
