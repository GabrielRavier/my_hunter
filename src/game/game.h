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

// This is one of the ducks you shoot at
// color is one of 0 (black), 1 (red) or 2 (blue)
// draw_shoot_rectangle determines whether we should be drawing the white
// rectangle corresponding to the duck (when shooting)
// score_text is the text we draw when the duck is shot
struct session_duck {
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

// This is one of the ducks at the bottom of the screen, at the right of "HIT"
struct round_duck {
    enum round_duck_state {
        ROUND_DUCK_STATE_LIVES,
        ROUND_DUCK_STATE_DEAD,
        ROUND_DUCK_STATE_FLYING,
        ROUND_DUCK_STATE_END_ROUND_FLASHING,
    } state;
    sfSprite *sprite;
};

// This is a sound with its associated buffer
struct sound_with_buffer {
    sfSoundBuffer *buffer;
    sfSound *sf_sound;
};

// This is the textures we use
struct game_textures {
    sfTexture *title;
    sfTexture *cursor;
    sfTexture *background;
    sfTexture *ducks;
    sfTexture *dog;
};

// This is the sprites we display
struct game_sprites {
    sfSprite *title_background;
    sfSprite *title_cursor;
    sfSprite *gameplay_background;
    sfSprite *fly_away;
    sfSprite *perfect_box;
    sfSprite *text_box;
    sfSprite *dog;
};

// This is the top and current score, stored as ints and text
struct game_scores {
    int32_t top_as_int;
    sfText *top_as_text;
    int32_t current_as_int;
    sfText *current_as_text;
};

// This is the sounds we use
struct game_sounds {
    struct sound_with_buffer duck;
    struct sound_with_buffer flying;
    struct sound_with_buffer gun_shoot;
    struct sound_with_buffer duck_falling;
    struct sound_with_buffer duck_thud;
    struct sound_with_buffer gottem;
    struct sound_with_buffer dog_mocking;
    struct sound_with_buffer round_ducks_move;
    struct sound_with_buffer end_round_success;
    struct sound_with_buffer game_over;
    struct sound_with_buffer game_over_dog;
    struct sound_with_buffer perfect;
};

// This is the state for a session
// clear_screen_for_shoot determines whether we just shot and thus need to clear
// the screen
// last_duck_fall_x_position is the x position of the latest place a duck fell
// at
// shots_left is the amount of ammunition the player has left
struct game_session_state {
    struct session_duck ducks[2];
    bool clear_screen_for_shoot;
    float last_duck_fall_x_position;
    int shots_left;
};

// This is the state for a round
// current_ducks_index is the index of the ducks we're currently working with in
// terms of the bottom of the screen, e.g. if we've already done 3 sessions in 2
// ducks mode, this will be 5
// frame_finished_sorting is used by GAME_END_ROUND to determine on which frame
// it finished doing the sorting of all the dead ducks
// ducks is the ducks from the bottom of the screen
struct game_round_state {
    size_t current_ducks_index;
    intmax_t frame_finished_sorting;
    struct round_duck ducks[10];
};

// This is the round we currently are on, stored as ints and text
struct game_round {
    int as_int;
    sfText *as_text;
};

// This is the state for the title
// selected_game is either 0, 1, or 2 to A, B or C
struct game_title_state {
    int selected_game;
};

// This is the text box we sometimes draw
struct game_text_box {
    bool should_draw;
    sfText *text;
};

// This is the state of the game
struct game_state {
    struct game_scores scores;
    struct game_session_state session;
    struct game_round_state round;
    struct game_round current_round;
    struct game_title_state title;
    struct game_text_box text_box;
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
    sfMusic *current_music;
};

// This is all the resources we use in the game
struct game_resources {
    sfFont *nes_font;
    struct game_textures textures;
    struct game_sprites sprites;
    struct game_sounds sounds;
};

// This is the main struct for the entire game
struct game {
    sfRenderWindow *window;
    struct game_resources resources;
    struct game_state state;
};

void game_main_loop(struct game *self);
void game_destroy(struct game *self);
