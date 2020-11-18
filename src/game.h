/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game
*/

#pragma once

#include <SFML/Graphics.h>
#include <stdbool.h>

struct game {
    sfRenderWindow *window;
    int max_score;
};

bool game_create(struct game *self);
void game_main_loop(struct game *self);
void game_destroy(struct game *self);
