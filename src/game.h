/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game
*/

#pragma once

#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdint.h>
#include <stdbool.h>

struct game {
    sfRenderWindow *window;
    sfFont *nes_font;
    sfTexture *title_background_texture;
    sfSprite *title_background_sprite;
    int top_score;
    sfText *top_score_text;
    sfMusic *title_music;
    uintmax_t frames;
};

bool game_create(struct game *self);
void game_main_loop(struct game *self);
void game_destroy(struct game *self);
