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

struct duck {
    sfSprite *sprite;
    bool is_active;
    float angle;
    float speed;
    int color;
    bool got_shot;
};

// selected_game is either 0, 1, or 2 to A, B or C
struct game {
    sfRenderWindow *window;
    sfTexture *title_texture;
    sfTexture *cursor_texture;
    sfTexture *background_texture;
    sfTexture *ducks_texture;
    sfTexture *dog_texture;
    sfSprite *title_background_sprite;
    sfSprite *title_cursor_sprite;
    sfSprite *gameplay_background_sprite;
    bool should_draw_text_box;
    sfSprite *text_box_sprite;
    sfSprite *dog_sprite;
    struct duck ducks[2];
    sfFont *nes_font;
    int32_t top_score;
    sfText *top_score_text;
    int32_t current_score;
    sfText *current_score_text;
    int current_round;
    sfText *current_round_text;
    sfText *text_box_text;
    sfMusic *current_music;
    sfSoundBuffer *duck_sound_buffer;
    sfSound *duck_sound;
    sfSoundBuffer *flying_sound_buffer;
    sfSound *flying_sound;
    int selected_game;
    uintmax_t frames_since_mode_begin;
    int shoot_frames_left;
    enum game_mode {
        GAME_MODE_NONE,
        GAME_MODE_TITLE,
        GAME_MODE_START_ROUND,
        GAME_MODE_ROUND,
    } mode;
};

bool game_create(struct game *self);
void game_main_loop(struct game *self);
void game_destroy(struct game *self);
