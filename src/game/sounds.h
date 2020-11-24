/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_sounds
*/

#pragma once

#include "sound_with_buffer.h"

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
