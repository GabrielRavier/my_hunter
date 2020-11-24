/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares sound_with_buffer
*/

#pragma once

#include <SFML/Audio/Types.h>

// This is a sound with its associated buffer
struct sound_with_buffer {
    sfSoundBuffer *buffer;
    sfSound *sf_sound;
};
