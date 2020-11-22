/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements random.h
*/

#include "random.h"
#include "my/assert.h"
#include <stdlib.h>

static int rand_at_most(int max)
{
    const unsigned num_bins = (unsigned)max + 1;
    const unsigned num_rand = (unsigned)RAND_MAX + 1;
    const unsigned bin_size = num_rand / num_bins;
    const unsigned defect = num_rand % num_bins;
    int x;

    MY_ASSERT(max < RAND_MAX);
    do {
        x = rand();
    } while (num_rand - defect <= (unsigned)x);
    return (x / bin_size);
}

int random_int_between(int min, int max)
{
    const int range = max - min;

    MY_ASSERT(min <= max);
    return (rand_at_most(range) + min);
}

float random_float_between(float min, float max)
{
    const float scale = rand() / (float)RAND_MAX;

    MY_ASSERT(min <= max);
    return min + scale * (max - min);
}
