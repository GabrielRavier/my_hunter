/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_text_box
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include <stdbool.h>

// This is the text box we sometimes draw
struct game_text_box {
    bool should_draw;
    sfText *text;
};
