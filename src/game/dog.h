/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares dog methods
*/

#pragma once

#include <SFML/Graphics/Types.h>
#include <stdint.h>

void dog_update_walking(sfSprite *self);
void dog_update_flying(sfSprite *self,
    uintmax_t frames_since_mode_begin);
