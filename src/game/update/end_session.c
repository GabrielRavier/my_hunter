/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_end_session
*/

#include "end_session.h"
#include "../set/mode.h"
#include "my/macros.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/System/Vector2.h>

static const int MOCKING_X_POSITION = 112;
static const int DOG_CUTSCENE_INITIAL_Y_POSITION = 161;
static const sfIntRect GOT_ONE_RECT = {10 - 22, 264, 44 - (10 - 22), 304 - 264};
static const sfIntRect GOT_TWO_RECT = {52, 264, 108 - 52, 304 - 264};
static const int MOCKING_RECT_BASE_LEFT = 2;
static const int MOCKING_RECT_LEFT_PER_SPRITE = (34 - MOCKING_RECT_BASE_LEFT);
static const int MOCKING_RECT_TOP = 320;
static const int MOCKING_RECT_WIDTH = 31 - MOCKING_RECT_BASE_LEFT;
static const int MOCKING_RECT_HEIGHT = 360 - MOCKING_RECT_TOP;
static const uintmax_t FRAMES_PER_MOCKING_RECT_CHANGE = 5;
static const uintmax_t FRAME_DOG_CUTSCENE_START = 50;
static const uintmax_t FRAME_DOG_CUTSCENE_STOP_GOING_UP = 77;
static const uintmax_t FRAME_DOG_CUTSCENE_START_GOING_DOWN = 97;
static const uintmax_t FRAME_DOG_CUTSCENE_STOP_GOING_DOWN = 120;
static const uintmax_t FRAME_DOG_CUTSCENE_END = 150;

static void start_dog_cutscene(struct game *self, int ducks_dead)
{
    self->state.session.last_duck_fall_x_position =
        MY_CLAMP(self->state.session.last_duck_fall_x_position, 44, 132);
    sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
        ducks_dead != 0 ? self->state.session.last_duck_fall_x_position :
        MOCKING_X_POSITION, DOG_CUTSCENE_INITIAL_Y_POSITION});
    if (ducks_dead == 1)
        sfSprite_setTextureRect(self->resources.sprites.dog, GOT_ONE_RECT);
    if (ducks_dead >= 2)
        sfSprite_setTextureRect(self->resources.sprites.dog, GOT_TWO_RECT);
    sfSound_play(((ducks_dead > 0) ? self->resources.sounds.gottem :
        self->resources.sounds.dog_mocking).sf_sound);
}

static void update_dog_cutscene(struct game *self, int ducks_dead)
{
    sfVector2f dog_position = sfSprite_getPosition(self->resources.sprites.dog);

    if (ducks_dead == 0)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            (sfIntRect){MOCKING_RECT_BASE_LEFT +
                (((self->state.frames_since_mode_begin %
                (FRAMES_PER_MOCKING_RECT_CHANGE * 2)) <
                FRAMES_PER_MOCKING_RECT_CHANGE) * MOCKING_RECT_LEFT_PER_SPRITE),
                MOCKING_RECT_TOP, MOCKING_RECT_WIDTH, MOCKING_RECT_HEIGHT});
    dog_position.y -= (self->state.frames_since_mode_begin <
        FRAME_DOG_CUTSCENE_STOP_GOING_UP) * 2;
    dog_position.y += (self->state.frames_since_mode_begin >
        FRAME_DOG_CUTSCENE_START_GOING_DOWN &&
        self->state.frames_since_mode_begin <
        FRAME_DOG_CUTSCENE_STOP_GOING_DOWN) * 2;
    sfSprite_setPosition(self->resources.sprites.dog, dog_position);
}

void game_update_end_session(struct game *self)
{
    int ducks_dead = 0;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        ducks_dead += (self->state.session.ducks[i].state == DUCK_STATE_DEAD);
    if (self->state.frames_since_mode_begin == FRAME_DOG_CUTSCENE_START)
        start_dog_cutscene(self, ducks_dead);
    if (self->state.frames_since_mode_begin > FRAME_DOG_CUTSCENE_START)
        update_dog_cutscene(self, ducks_dead);
    if (self->state.frames_since_mode_begin > FRAME_DOG_CUTSCENE_END)
        game_set_mode(self, GAME_MODE_SESSION);
}
