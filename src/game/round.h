/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_round
*/

#pragma once

#include <SFML/Graphics/Types.h>

// This is the round we currently are on, stored as ints and text
struct game_round {
    int as_int;
    sfText *as_text;
};
