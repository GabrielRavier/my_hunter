/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_scores
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include <stdint.h>

// This is the top and current score, stored as ints and text
struct game_scores {
    int32_t top_as_int;
    sfText *top_as_text;
    int32_t current_as_int;
    sfText *current_as_text;
};
