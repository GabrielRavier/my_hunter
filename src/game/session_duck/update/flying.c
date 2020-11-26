/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements session_duck_update_flying
*/

#include "internal.h"
#include "../update.h"
#include "../set.h"
#include "../../../random.h"
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Sprite.h>
#include <math.h>

static const sfIntRect rects_flying[3][6] = {
    {
        {4, 2, 35 - 4, 33 - 2},
        {41, 2, 71 - 41, 33 - 2},
        {72, 2, 98 - 72, 35 - 2},
        {109, 8, 143 - 109, 37 - 8},
        {147, 8, 181 - 147, 38 - 8},
        {184, 8, 218 - 184, 40 - 8}
    },
    {
        {8, 45, 35 - 8, 76 - 45},
        {39, 45, 71 - 39, 76 - 45},
        {73, 45, 98 - 73, 78 - 45},
        {107, 48, 141 - 107, 77 - 48},
        {145, 48, 179 - 145, 78 - 48},
        {182, 48, 216 - 182, 80 - 48}
    },
    {
        {6, 88, 33 - 6, 119 - 88},
        {37, 88, 69 - 37, 119 - 88},
        {71, 88, 96 - 71, 121 - 88},
        {105, 91, 139 - 105, 120 - 91},
        {143, 91, 177 - 143, 121 - 91},
        {180, 91, 214 - 180, 123 - 91}
    }
};

static const int NES_SCREEN_WIDTH = 256;

static const int DUCK_HIGHEST_Y = 160;

static float get_random_angle_deviation(void)
{
    static const float DEVIATION = 0.25f;

    return random_float_between(-DEVIATION, DEVIATION);
}

// Does collision when normally flying (i.e. not in fly away mode or when the
// player has no ammunition)
static void do_normal_flying_collision(struct session_duck *self,
    struct game *game, sfFloatRect self_bounds)
{
    if (self_bounds.left < 0 && cosf(self->angle) < 0) {
        self->angle = self->angle + M_PI + get_random_angle_deviation();
        if (random_int_between(0, 5) != 0)
            self->angle = -self->angle;
        session_duck_update(self, game);
    }
    if ((self_bounds.left + self_bounds.width) > 256 && cosf(self->angle) > 0) {
        self->angle = self->angle + M_PI + get_random_angle_deviation();
        if (random_int_between(0, 5) != 0)
            self->angle = -self->angle;
        session_duck_update(self, game);
    }
    if (self_bounds.top < 0 && sinf(self->angle) < 0) {
        self->angle = -self->angle + get_random_angle_deviation();
        session_duck_update(self, game);
    }
}

static void do_collision(struct session_duck *self, struct game *game)
{
    sfFloatRect self_bounds = sfSprite_getGlobalBounds(self->sprite);
    
    if (game->state.session.shots_left == 0 ||
        game->state.mode == GAME_MODE_SESSION_FLY_AWAY) {
        if (((self_bounds.left + self_bounds.width) < 0) ||
            (self_bounds.left > NES_SCREEN_WIDTH) ||
            ((self_bounds.top + self_bounds.height) < 0))
            session_duck_set_state(self, game, DUCK_STATE_INACTIVE);
    } else
        do_normal_flying_collision(self, game, self_bounds);
    if ((self_bounds.top + self_bounds.height) > DUCK_HIGHEST_Y &&
        sinf(self->angle > 0)) {
        self->angle = -self->angle + get_random_angle_deviation();
        session_duck_update(self, game);
    }
}

void session_duck_update_flying(struct session_duck *self,
    struct game *game, sfVector2f current_position)
{
    const int which_sprite = ((game->state.frames_since_mode_begin % (3 + 3 +
        5)) >= 3) + ((game->state.frames_since_mode_begin % (3 + 3 + 5)) >=
        (3 + 3));
    sfIntRect final_rect;
    
    if (game->state.session.shots_left == 0)
        self->angle = -M_PI / 2;
    sfSprite_setPosition(self->sprite, (sfVector2f){
            current_position.x + (self->speed * cosf(self->angle)),
            current_position.y + (self->speed * sinf(self->angle))});
    if (fabsf(sinf(self->angle)) < fabsf(cosf(self->angle)))
        final_rect = rects_flying[self->color][which_sprite + 3];
    else
        final_rect = rects_flying[self->color][which_sprite];
    if (cosf(self->angle) < 0)
        final_rect = (sfIntRect){final_rect.left + final_rect.width,
            final_rect.top, -final_rect.width, final_rect.height};
    sfSprite_setTextureRect(self->sprite, final_rect);
    do_collision(self, game);
}
