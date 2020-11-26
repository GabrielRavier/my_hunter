/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_end_round_game_over
*/

#include "internal.h"
#include "../../set/mode.h"
#include "../../../text_utils.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Sprite.h>

static const sfVector2f GAME_OVER_INITIAL_DOG_POSITION = {112, 161};
static const uintmax_t FRAME_DRAW_TEXT_AND_PLAY_SOUND = 16;
static const uintmax_t FRAME_START_MOVING_DOG = 140;
static const uintmax_t FRAME_STOP_MOVING_DOG = 167;
static const uintmax_t FRAME_FINISH_GAME_OVER = 370;

static void draw_game_over_text_and_play_game_over_sound(struct game *self)
{
    sfSound_play(self->resources.sounds.game_over.sf_sound);
    self->state.text_box.should_draw = true;
    text_set_printf(self->state.text_box.text, "GAME\n\n OVER");
}

static void play_game_over_dog_sound_and_place_dog(struct game_resources *self)
{
    sfSound_play(self->sounds.game_over_dog.sf_sound);
    sfSprite_setPosition(self->sprites.dog, GAME_OVER_INITIAL_DOG_POSITION);
}

void game_update_end_round_game_over(struct game *self)
{
    const uintmax_t frames_since_finished_sorting =
        self->state.frames_since_mode_begin -
        self->state.round.frame_finished_sorting;
    const sfIntRect dog_rect = {2 + (((self->state.frames_since_mode_begin %
       17) < 12) * (34 - 2)), 320, 31 - 2, 360 - 320};

    if (frames_since_finished_sorting == FRAME_DRAW_TEXT_AND_PLAY_SOUND)
        draw_game_over_text_and_play_game_over_sound(self);
    if (frames_since_finished_sorting == FRAME_START_MOVING_DOG)
        play_game_over_dog_sound_and_place_dog(&self->resources);
    if (frames_since_finished_sorting > FRAME_START_MOVING_DOG &&
        frames_since_finished_sorting < FRAME_STOP_MOVING_DOG) {
        sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
            sfSprite_getPosition(self->resources.sprites.dog).x,
            sfSprite_getPosition(self->resources.sprites.dog).y - 2});
    }
    if (frames_since_finished_sorting == FRAME_FINISH_GAME_OVER)
        game_set_mode(self, GAME_MODE_TITLE);
    sfSprite_setTextureRect(self->resources.sprites.dog, dog_rect);
}
