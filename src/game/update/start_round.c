/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_start_round
*/

#include "start_round.h"
#include "../change_music.h"
#include "../dog.h"
#include "../set/mode.h"
#include <SFML/Graphics/Sprite.h>

static const uintmax_t FRAME_DOG_FIRST_SEARCH_START = 120;
static const uintmax_t FRAME_TEXT_BOX_GO_AWAY = 130;
static const uintmax_t FRAME_DOG_FIRST_SEARCH_END = 170;
static const uintmax_t FRAME_DOG_SECOND_SEARCH_START = 285;
static const uintmax_t FRAME_DOG_SECOND_SEACH_END = 335;
static const uintmax_t FRAME_DOG_START_SURPRISED = 343;
static const uintmax_t FRAME_DOG_START_JUMP = 368;
static const uintmax_t FRAME_START_SESSION = 468;
static const uintmax_t FRAME_DOG_START_GOING_DOWN = (FRAME_DOG_START_JUMP +
    18);
static const uintmax_t FRAMES_PER_DOG_WALK_STEP = 7;
static const uintmax_t FRAMES_PER_DOG_SEARCH_HEAD_MOVE = 14;
static const sfIntRect DOG_RECT_SEARCH_1 = {-1, 69, 56, 112 - 69};
static const sfIntRect DOG_RECT_SEARCH_2 = {55, 69, 109 - 55, 112 - 69};
static const sfIntRect DOG_RECT_SURPRISED = {0, 121, 53, 169 - 121};
static const sfIntRect DOG_RECT_JUMP_GOING_DOWN = {40, 185, 37, 169 - 121};
static const sfIntRect DOG_RECT_JUMP_GOING_UP = {0, 185, 37, 169 - 121};
static const int DOG_JUMP_INITIAL_Y = 119;
static const int DOG_SURPRISED_Y = 131;

static void do_walk_step(struct game *self)
{
    if (self->state.frames_since_mode_begin > FRAME_DOG_START_SURPRISED) {
        sfSprite_setTextureRect(self->resources.sprites.dog,
            DOG_RECT_SURPRISED);
        sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
            sfSprite_getPosition(self->resources.sprites.dog).x,
            DOG_SURPRISED_Y});
    }
    else if (!(self->state.frames_since_mode_begin >
        FRAME_DOG_FIRST_SEARCH_START && self->state.frames_since_mode_begin <
        FRAME_DOG_FIRST_SEARCH_END) && !(self->state.frames_since_mode_begin >
        FRAME_DOG_SECOND_SEARCH_START && self->state.frames_since_mode_begin <
        FRAME_DOG_SECOND_SEACH_END))
        dog_update_walking(self->resources.sprites.dog);
    else
        sfSprite_setTextureRect(self->resources.sprites.dog,
            ((self->state.frames_since_mode_begin %
            FRAMES_PER_DOG_SEARCH_HEAD_MOVE) != 0) ? DOG_RECT_SEARCH_1 :
                DOG_RECT_SEARCH_2);
}

static void do_start_jump(struct game *self)
{
    sfSprite_setTextureRect(self->resources.sprites.dog,
        DOG_RECT_JUMP_GOING_UP);
    sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
        sfSprite_getPosition(self->resources.sprites.dog).x,
        DOG_JUMP_INITIAL_Y});
    game_change_music(&self->state.current_music, "assets/bark.ogg");
}

void game_update_start_round(struct game *self)
{
    if (self->state.frames_since_mode_begin > FRAME_TEXT_BOX_GO_AWAY)
        self->state.text_box.should_draw = false;
    if ((self->state.frames_since_mode_begin < FRAME_DOG_START_JUMP) &&
        (self->state.frames_since_mode_begin % FRAMES_PER_DOG_WALK_STEP) == 0)
        do_walk_step(self);
    if (self->state.frames_since_mode_begin == FRAME_DOG_START_JUMP)
        do_start_jump(self);
    if (self->state.frames_since_mode_begin > FRAME_DOG_START_JUMP) {
        if (self->state.frames_since_mode_begin == FRAME_DOG_START_GOING_DOWN)
            sfSprite_setTextureRect(self->resources.sprites.dog,
                DOG_RECT_JUMP_GOING_DOWN);
        dog_update_flying(self->resources.sprites.dog,
            self->state.frames_since_mode_begin);
    }
    if (self->state.frames_since_mode_begin > FRAME_START_SESSION)
        game_set_mode(self, GAME_MODE_SESSION);
}
