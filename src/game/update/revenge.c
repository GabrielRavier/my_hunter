/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_revenge
*/

#include "revenge.h"
#include "../set/mode.h"
#include "my/macros.h"
#include <SFML/Audio/Music.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>

static const uintmax_t FRAME_SECOND_JUST_SHOT_RECT = 8;
static const sfIntRect SECOND_JUST_SHOT_RECT = {168, 376, 189 - 168, 406 - 376};
static const uintmax_t FRAME_THIRD_JUST_SHOT_RECT =
    FRAME_SECOND_JUST_SHOT_RECT * 2;
static const sfIntRect THIRD_JUST_SHOT_RECT = {192, 376, 213 - 192, 406 - 376};
static const uintmax_t FRAME_START_FALLING = FRAME_THIRD_JUST_SHOT_RECT + 4;
static const float FALL_Y[] = {1, 1, 0, 1, 1, 1, 1, 1, 1, 2, 5, 5, 2, 3, 1000};
static const uintmax_t FRAME_START_WALKING = FRAME_START_FALLING +
    MY_ARRAY_SIZE(FALL_Y) + 10;
static const sfVector2f INITIAL_WALKING_POINT = {230, 140};
static const sfIntRect RECTS_WALKING[] = {
    {144, 439, 161 - 144, 480 - 439},
    {168, 439, 192 - 168, 480 - 439},
    {200, 439, 217 - 200, 480 - 439},
    {224, 439, 248 - 224, 480 - 439}
};
static const uintmax_t FRAME_TURN_TOWARDS_PLAYER = FRAME_START_WALKING + 190;
static const sfIntRect RECT_TURN_TOWARDS_PLAYER = {256, 439, 272 - 256, 480 - 439};
static const uintmax_t FRAME_START_CUTSCENE = FRAME_TURN_TOWARDS_PLAYER + 5;
static const sfIntRect RECTS_CUTSCENE[] = {
    {280, 437, 305 - 280, 480 - 437},
    {305, 437, 330 - 305, 480 - 437},
    {336, 437, 361 - 336, 480 - 437},
    {305, 437, 330 - 305, 480 - 437}
};
static const uintmax_t FRAME_END_CUTSCENE = FRAME_START_CUTSCENE + 175;
static const sfVector2f POSITION_OUT_OF_SCREEN = {1000, 1000};
static const uintmax_t FRAME_START_SESSION = FRAME_END_CUTSCENE + 30;
static const sfVector2f TEXT_POSITION = {30, 100};

static void do_fall(struct game *self)
{
    sfVector2f dog_position = sfSprite_getPosition(self->resources.sprites.dog);
    uintmax_t current_fall_frame = self->state.frames_since_mode_begin -
        FRAME_START_FALLING - 1;

    if (current_fall_frame < MY_ARRAY_SIZE(FALL_Y))
        dog_position.y += FALL_Y[current_fall_frame];
    sfSprite_setPosition(self->resources.sprites.dog, dog_position);
}

static void advance_walking_dog(struct game *self)
{
    sfVector2f current_dog_position;

    if ((self->state.frames_since_mode_begin % 2) == 0) {
        current_dog_position = sfSprite_getPosition(
            self->resources.sprites.dog);
        --current_dog_position.x;
        sfSprite_setPosition(self->resources.sprites.dog, current_dog_position);
    }
    if ((self->state.frames_since_mode_begin % 4) == 0)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            RECTS_WALKING[(self->state.frames_since_mode_begin % 16) / 4]);
}

static void do_cutscene(struct game *self)
{
    if (self->state.frames_since_mode_begin == FRAME_TURN_TOWARDS_PLAYER) {
        sfSprite_setTextureRect(self->resources.sprites.dog,
            RECT_TURN_TOWARDS_PLAYER);
        sfText_setPosition(self->state.revenge.text, TEXT_POSITION);
    }
    if (self->state.frames_since_mode_begin > FRAME_START_CUTSCENE &&
        ((self->state.frames_since_mode_begin % 4) == 0))
        sfSprite_setTextureRect(self->resources.sprites.dog,
            RECTS_CUTSCENE[(self->state.frames_since_mode_begin % 16) / 4]);
    if (self->state.frames_since_mode_begin == FRAME_END_CUTSCENE) {
        sfMusic_stop(self->state.current_music);
        sfSprite_setPosition(self->resources.sprites.dog,
            POSITION_OUT_OF_SCREEN);
        sfText_setPosition(self->state.revenge.text, POSITION_OUT_OF_SCREEN);
    }
    if (self->state.frames_since_mode_begin == FRAME_START_SESSION)
        game_set_mode(self, GAME_MODE_SESSION);
}

void game_update_revenge(struct game *self)
{
    if (self->state.frames_since_mode_begin == FRAME_SECOND_JUST_SHOT_RECT)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            SECOND_JUST_SHOT_RECT);
    if (self->state.frames_since_mode_begin == FRAME_THIRD_JUST_SHOT_RECT)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            THIRD_JUST_SHOT_RECT);
    if (self->state.frames_since_mode_begin > FRAME_START_FALLING)
        do_fall(self);
    if (self->state.frames_since_mode_begin == FRAME_START_WALKING) {
        sfSprite_setTextureRect(self->resources.sprites.dog, RECTS_WALKING[0]);
        sfSprite_setPosition(self->resources.sprites.dog,
            INITIAL_WALKING_POINT);
    }
    if (self->state.frames_since_mode_begin >= FRAME_START_WALKING &&
        self->state.frames_since_mode_begin < FRAME_TURN_TOWARDS_PLAYER)
        advance_walking_dog(self);
    do_cutscene(self);
}
