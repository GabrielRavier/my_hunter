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

static void start_dog_cutscene(struct game *self, int ducks_dead)
{
    if (self->state.session.last_duck_fall_x_position < 44)
        self->state.session.last_duck_fall_x_position = 44;
    if (self->state.session.last_duck_fall_x_position > 132)
        self->state.session.last_duck_fall_x_position = 132;
    sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
        ducks_dead != 0 ? self->state.session.last_duck_fall_x_position : 112,
            161});
    if (ducks_dead == 1)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            (sfIntRect){10 - 22, 264, 44 - (10 - 22), 304 - 264});
    if (ducks_dead >= 2)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            (sfIntRect){52, 264, 108 - 52, 304 - 264});
    sfSound_play(((ducks_dead > 0) ? self->resources.sounds.gottem :
        self->resources.sounds.dog_mocking).sf_sound);
}

static void update_dog_cutscene(struct game *self, int ducks_dead)
{
    sfVector2f dog_position = sfSprite_getPosition(self->resources.sprites.dog);

    if (ducks_dead == 0)
        sfSprite_setTextureRect(self->resources.sprites.dog,
            (sfIntRect){2 + (((self->state.frames_since_mode_begin % 10) < 5) *
                (34 - 2)), 320, 31 - 2, 360 - 320});
    dog_position.y -= (self->state.frames_since_mode_begin < 77) * 2;
    dog_position.y += (self->state.frames_since_mode_begin > 97 &&
        self->state.frames_since_mode_begin < 120) * 2;
    sfSprite_setPosition(self->resources.sprites.dog, dog_position);
}

void game_update_end_session(struct game *self)
{
    int ducks_dead = 0;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        ducks_dead += (self->state.session.ducks[i].state == DUCK_STATE_DEAD);
    if (self->state.frames_since_mode_begin == 50)
        start_dog_cutscene(self, ducks_dead);
    if (self->state.frames_since_mode_begin > 50)
        update_dog_cutscene(self, ducks_dead);
    if (self->state.frames_since_mode_begin > 150)
        game_set_mode(self, GAME_MODE_SESSION);
}
