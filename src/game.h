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

// selected_game is either 0, 1, or 2 to A, B or C
struct game {
    sfRenderWindow *window;
    sfTexture *title_texture;
    sfTexture *cursor_texture;
    sfTexture *background_texture;
    sfSprite *title_background_sprite;
    sfSprite *title_cursor_sprite;
    sfSprite *gameplay_background_sprite;
    sfFont *nes_font;
    int32_t top_score;
    sfText *top_score_text;
    int32_t current_score;
    sfText *current_score_text;
    int current_round;
    sfText *current_round_text;
    sfMusic *current_music;
    int selected_game;
    uintmax_t frames_since_mode_begin;
    enum game_mode {
        GAME_MODE_NONE,
        GAME_MODE_TITLE,
        GAME_MODE_START_ROUND,
    } mode;
};

bool game_create(struct game *self);
void game_main_loop(struct game *self);
void game_destroy(struct game *self);
