/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_sounds
*/

#include "internal.h"
#include "my/macros.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Audio/SoundBuffer.h>

static bool make_sound(struct sound_with_buffer *sound, const char *filename)
{
    sound->buffer = sfSoundBuffer_createFromFile(filename);
    if (sound->buffer == NULL)
        return (false);
    sound->sf_sound = sfSound_create();
    if (sound->sf_sound == NULL)
        return (false);
    sfSound_setBuffer(sound->sf_sound, sound->buffer);
    return (true);
}

bool make_sounds(struct game_sounds *self)
{
    struct {
        struct sound_with_buffer *ptr;
        const char *filename;
    } sounds_to_make[] = {{&self->duck, "assets/duck.wav"}, {&self->flying,
        "assets/flying.wav"}, {&self->gun_shoot, "assets/gun_shoot.wav"},
        {&self->duck_falling, "assets/duck_falling.wav"}, {&self->duck_thud,
        "assets/duck_thud.wav"}, {&self->gottem, "assets/gottem.wav"},
        {&self->dog_mocking, "assets/dog_mocking.wav"},
        {&self->round_ducks_move, "assets/round_ducks_move.wav"},
        {&self->end_round_success, "assets/end_round_success.wav"},
        {&self->game_over, "assets/game_over.wav"}, {&self->game_over_dog,
        "assets/game_over_dog.wav"}, {&self->perfect, "assets/perfect.wav"}};

    for (size_t i = 0; i < MY_ARRAY_SIZE(sounds_to_make); ++i)
        if (!make_sound(sounds_to_make[i].ptr, sounds_to_make[i].filename))
            return (false);
    sfSound_setLoop(self->duck.sf_sound, true);
    sfSound_setLoop(self->flying.sf_sound, true);
    return (true);
}
