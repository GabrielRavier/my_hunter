/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_get_duck_speed
*/

#include "get_duck_speed.h"
#include "my/macros.h"

static const float BLUE_DUCK_SPEED_MULTIPLER = 1.25f;
static const float RED_DUCK_SPEED_MULTIPLIER = 1.5f;
static const float INITIAL_DUCK_SPEED = 1.5f;
static const float MULTIPLIER_AFTER_10TH = 6.f;

static void do_current_round(float *result, int current_round)
{
    static const float multipliers[] = {1.0f, 1.0f, 1.07f, 1.2f, 1.5f, 1.65f,
        2.f, 2.3f, 2.5f, 3.f, 3.5f};
    
    if ((size_t)current_round < MY_ARRAY_SIZE(multipliers))
        *result *= multipliers[current_round];
    else
        *result *= MULTIPLIER_AFTER_10TH;
}

static void do_color(float *result, int color)
{
    if (color == 2)
        *result *= BLUE_DUCK_SPEED_MULTIPLER;
    if (color == 1)
        *result *= RED_DUCK_SPEED_MULTIPLIER;
}

float game_get_duck_speed(struct game_state *self, struct session_duck *duck)
{
    float result = INITIAL_DUCK_SPEED;

    do_current_round(&result, self->current_round.as_int);
    do_color(&result, duck->color);
    return result;
}
