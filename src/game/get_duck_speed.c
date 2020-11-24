/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_get_duck_speed
*/

#include "get_duck_speed.h"
#include "my/macros.h"

static void do_current_round(float *result, int current_round)
{
    float multipliers[] = {1.0f, 1.0f, 1.07f, 1.2f, 1.5f, 1.65f, 2.f, 2.3f,
        2.5f, 3.f, 3.5f};
    
    if ((size_t)current_round < MY_ARRAY_SIZE(multipliers))
        *result *= multipliers[current_round];
    else
        *result *= 6.f;
}

static void do_color(float *result, int color)
{
    if (color == 2)
        *result *= 1.25f;
    if (color == 1)
        *result *= 1.5f;
}

float game_get_duck_speed(struct game_state *self,
    struct session_duck *duck)
{
    float result = 1.5f;

    do_current_round(&result, self->current_round.as_int);
    do_color(&result, duck->color);
    return result;
}
