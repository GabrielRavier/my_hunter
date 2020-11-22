/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include <SFML/Audio/Types.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct duck {
    sfSprite *sprite;
    enum duck_state {
        DUCK_STATE_INACTIVE,
        DUCK_STATE_FLYING,
        DUCK_STATE_FALLING,
        DUCK_STATE_DEAD,
    } state;
    int frames_since_state_change;
    float angle;
    float speed;
    int color;
    bool draw_shoot_rectangle;
    sfText *score_text;
};

struct round_duck {
    enum round_duck_state {
        ROUND_DUCK_STATE_LIVES,
        ROUND_DUCK_STATE_DEAD,
        ROUND_DUCK_STATE_FLYING,
        ROUND_DUCK_STATE_END_ROUND_FLASHING,
    } state;
    sfSprite *sprite;
};

struct sound_with_buffer {
    sfSoundBuffer *buffer;
    sfSound *sf_sound;
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
    sfSprite *fly_away_sprite;
    sfSprite *perfect_box_sprite;
    bool should_draw_text_box;
    sfSprite *text_box_sprite;
    sfSprite *dog_sprite;
    size_t current_round_ducks_index;
    struct round_duck round_ducks[10];
    sfFont *nes_font;
    struct duck ducks[2];
    float last_duck_fall_x_position;
    intmax_t frame_finished_sorting;
    bool clear_screen_for_shoot;
    int32_t top_score;
    sfText *top_score_text;
    int32_t current_score;
    sfText *current_score_text;
    int current_round;
    sfText *current_round_text;
    sfText *text_box_text;
    sfMusic *current_music;
    struct sound_with_buffer duck_sound;
    struct sound_with_buffer flying_sound;
    struct sound_with_buffer gun_shoot_sound;
    struct sound_with_buffer duck_falling_sound;
    struct sound_with_buffer duck_thud_sound;
    struct sound_with_buffer gottem_sound;
    struct sound_with_buffer dog_mocking_sound;
    struct sound_with_buffer round_ducks_move_sound;
    struct sound_with_buffer end_round_success_sound;
    struct sound_with_buffer game_over_sound;
    struct sound_with_buffer game_over_dog_sound;
    struct sound_with_buffer perfect_sound;
    int shots_left;
    int selected_game;
    uintmax_t frames_since_mode_begin;
    enum game_mode {
        GAME_MODE_NONE,
        GAME_MODE_TITLE,
        GAME_MODE_START_ROUND,
        GAME_MODE_SESSION,
        GAME_MODE_SESSION_FLY_AWAY,
        GAME_MODE_END_SESSION,
        GAME_MODE_END_ROUND
    } mode;
};

void game_main_loop(struct game *self);
void game_destroy(struct game *self);
